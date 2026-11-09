

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "notation.h"
#include "game.h"
#include "board.h"

/**
 * @brief translate rank from game rank to array index
 * @param rank the rank looking to be translated to array syntax
 * @return the array index of the rank
 */
int translate_rank(char rank) {
  switch (rank) {
    case '8':
      return 0;
    case '7':
      return 1;
    case '6':
      return 2;
    case '5':
      return 3;
    case '4':
      return 4;
    case '3':
      return 5;
    case '2':
      return 6;
    case '1':
      return 7;
    default:
      fprintf(stderr,
              "error translating rank of a move from char to the corresponding "
              "numerical rank -> %c",
              rank);
      abort();
  }
}


int translate_file(char file) {
  switch (file) {
    case 'a':
      return 0;
    case 'b':
      return 1;
    case 'c':
      return 2;
    case 'd':
      return 3;
    case 'e':
      return 4;
    case 'f':
      return 5;
    case 'g':
      return 6;
    case 'h':
      return 7;
    default:
      fprintf(stderr,
              "error translating file of a move from char to the corresponding "
              "numerical file -> %c",
              file);
      abort();
  }
}

// 
char* find_square_name(int rank, int file) {
  char *square_name = "";
  // rank
  switch (rank) {
    case '0':
      square_name[0] = '8';
      break;
    case '1':
      square_name[0] = '7';
      break;
    case '2':
      square_name[0] = '6';
      break;
    case '3':
      square_name[0] = '5';
      break;
    case '4':
      square_name[0] = '4';
      break;
    case '5':
      square_name[0] = '3';
      break;
    case '6':
      square_name[0] = '2';
      break;
    case '7':
      square_name[0] = '1';
      break;
  }

  switch (file) {
    case '0':
      square_name[1] = 'a';
      break;
    case '1':
      square_name[1] = 'b';
      break;
    case '2':
      square_name[1] = 'c';
      break;
    case '3':
      square_name[1] = 'd';
      break;
    case '4':
      square_name[1] = 'e';
      break;
    case '5':
      square_name[1] = 'f';
      break;
    case '6':
      square_name[1] = 'g';
      break;
    case '7':
      square_name[1] = 'h';
      break;
  }
  return square_name;
}

char* generate_FEN(struct State state) {
  char *fen = malloc(FEN_MAX);
  if (fen == NULL) return NULL;

  int n = 0;

  // piece placement
  for (int i = 0; i < ROWS; i++) {
    int blanks = 0;
    for (int j = 0; j < COLS; j++) {
      char c = state.board[i][j];
      if (c == ' ') {
        blanks++;
      } else {
        if (blanks > 0) {
          fen[n++] = '0' + blanks;
          blanks = 0;
        }
        fen[n++] = c;
      }
    }
    if (blanks > 0) fen[n++] = '0' + blanks;
    if (i < ROWS - 1) fen[n++] = '/';
  }

  // current turn
  fen[n++] = ' ';
  fen[n++] = state.current_turn;

  // castling availability
  fen[n++] = ' ';
  int castle_start = n;
  for (size_t i = 0; state.castling_availability[i] != '\0' &&
                     state.castling_availability[i] != ' ' &&
                     state.castling_availability[i] != '-'; i++) {
    fen[n++] = state.castling_availability[i];
  }
  if (n == castle_start) fen[n++] = '-';

  // en passant target
  fen[n++] = ' ';
  if (state.en_passant_target[0] == '-' ||
      state.en_passant_target[0] == '\0' ||
      state.en_passant_target[0] == ' ') {
    fen[n++] = '-';
  } else {
    fen[n++] = state.en_passant_target[0];
    fen[n++] = state.en_passant_target[1];
  }

  // halfmove and fullmove clocks (also writes the null terminator)
  snprintf(fen + n, FEN_MAX - n, " %d %d",
           state.halfmove_clock, state.fullmove_clock);

  return fen;
}



// parse the fen string and create a board state object with all the right states
struct State build_state_from_fen(char fen[FEN_MAX]) {
  struct State new_state;
  strcpy(new_state.fen, fen);
  // printf("new_state.fen sizeof strlen: %d\n", (int) strlen(new_state.fen));
  // printf("build_state_from_fen strncpy result: %s\n", new_state.fen);

  char *token = strtok(fen, " ");
  // printf("board string taken from strtok: %s\n", token);
  char board_string[64]= "";
  int temp_num = 0;
  int i_board = 0;
  // parse board state
  for(int i = 0;i < strlen(token); i++) {
    // if number, iterate that many spaces
    if( isdigit(token[i]) != 0) {
      temp_num = atoi(&token[i]);
      for(int j = 0; j < temp_num; j++) {
        board_string[i_board++] = ' ';
      }
    } else if(token[i] == '/') {
      continue;
    } else {
      board_string[i_board++] = token[i];
    }
  }
  build_board_array(board_string, new_state.board);

  token = strtok(NULL, " ");
  new_state.current_turn = *token;

  token = strtok(NULL, " ");
  strcpy(new_state.castling_availability, token);

  token = strtok(NULL, " ");
  strcpy(new_state.en_passant_target, token);

  token = strtok(NULL, " ");
  new_state.halfmove_clock = atoi(token);

  token = strtok(NULL, " ");
  new_state.fullmove_clock = atoi(token);
 
  return new_state;
}
