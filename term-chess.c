#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FEN_MAX 95
// the columns that I want to reference
// #define a 0
// #define b 1
// #define c 2
// #define d 3
// #define e 4
// #define f 5
// #define g 6
// #define h 7
#define ROWS 8
#define COLS 8

struct State {
  char board[ROWS][COLS];
  char current_turn;
  char castling_availability[4];
  char en_passant_target[2];
  int halfmove_clock;
  int fullmove_clock;
  char fen[95];
};


void horizontal_separator() {
  printf("+---+---+---+---+---+---+---+---+\n");
}

void print_legend() {
  printf("  a   b   c   d   e   f   g   h\n\n");
}


void draw_board(char m[ROWS][COLS]) {
  horizontal_separator();
  for(int i = 0; i < ROWS; i++) {
    for(int j = 0; j < COLS; j++) {
      printf("| %c ", m[i][j]);
    }
    printf("| %d\n", ROWS - i);
    horizontal_separator();
  }
  print_legend();
}


void listen() {
  char move[10];
  if(fgets(move, sizeof(move), stdin) != NULL) {
    printf("move is %s", move);
  }
  return;
}

// Verify that the move is at least valid in the sense that it could happen independant
// of the actual current board state.
// Ex. a4 is a valid move because it's a pawn move on the a file to the 4th rank.
// but z32 is not valid because it does not exist. this will require parsing of the 
// piece specified (sometimes by the piece name like n for night or r for rook but sometimes rac5
// for the rook on the a file to c5) if required, pawns do not need it. if a piece is taken
// if check or mate occurs, if a piece is taken, etc.
//void parse_move() {
//}

// function called to move a piece from starting to ending square. This should be called
// after the verification of if it's a legal move takes place. That will be in the game
// logic file, this is just for board state and gui transformation.
//void move_piece(char m[ROWS][COLS], int start_row, int start_col, int end_row, int end_col) {
//}


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


// char* generate_board_state_for_FEN(char m[ROWS][COLS]) {
//
// }



// parse the fen string and create a board state object with all the right states
void build_state_from_fen(char fen[95]) {

}

int main() {

  struct State game_state = {
    .board = {
      {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
      {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
      {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
      {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
      {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
      {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
      {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
      {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    },
    .current_turn = 'w',
    .castling_availability = "KQkq",
    .en_passant_target = "-",
    .halfmove_clock = 0,
    .fullmove_clock = 1,
    .fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
  };

  draw_board(game_state.board);
  char *fen = generate_FEN(game_state);
  printf("Game FEN is: %s\n", fen);
  free(fen);
  return 0;
}
