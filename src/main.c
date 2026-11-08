#include "board.c"
#include "game.c"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>

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


void build_board_array(char *board_string, char board[8][8]) {
  int string_index = 0;
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      if(board_string[string_index]) {
        board[i][j] = board_string[string_index];
        string_index++;
      } else {
        fprintf(stderr, "Error with board string to board array parsing...\n");
        return;
      }
    }
  }

}

/* This function is for finding the actual squares that en_passant_target could
 * identify in the fen string. 
 * NOTE: this function assumes the move passed is a pawn move without capture */
char* check_and_find_en_passant(struct State state, char *pawn_move) {
  // Just find out if this move if a 2 space pawn move:
  // if yes -> return square name of the square skipped, (if black, [a-h]6; if
  //    white [a-h]3; take the first character of move for [a-h])
  // if no -> return "-"
  int file = translate_file(*pawn_move);
  int rank = translate_rank(*(pawn_move + 1));
  int conditional_black_rank = translate_rank('5');
  int conditional_white_rank = translate_rank('4');

  // check black pawn move (top so check first)
  if(rank == conditional_black_rank) {
    // check if the pawn moving to this rank is on file 6 or 7
    if(state.board[rank-1][file] == 'p') {
      return "-";
    } else {
      return find_square_name(rank-1, file);
    }
  }

  // check white pawn move
  if(rank == conditional_white_rank) {
    if(state.board[rank+1][file] == 'P') {
      return "-";
    } else {
      return find_square_name(rank + 1, file);
    }
  }
  unreachable();
}


/*
 *  Why do we need to parse the move if we are taking a char * and returning char *?
 *  What would parsing the move in a separate function give us? 
 *
 */
struct Move parse_move(char *move, char turn) {
  // options: 1. pawn move, just target square, must look for pawn in same rank and then move
  //          2. specified piece then target square
  //          3. specified piece with extra starting position identifier if 2 of the same piece could be the specified one.
  //          4. piece takes another piece (could also have extra specification)
  //          5. check
  //          6. checkmate
  struct Move current_move;

  if(strcmp(move, "O-O") == 0 || strcmp(move, "O-O-O") == 0) {
    current_move.is_castles = true;

    // provide info on if it's short or long castle
    if(strcmp(move, "O-O") == 0) {
      current_move.castles_short = true;
    } else {
      current_move.castles_short = false;
    }

    // provide turn information to help with validation and actual movement.
    if(turn == 'w') {
      current_move.piece = 'K';
    } else {
      current_move.piece = 'k'; // TODO - do I need to specify upper/lowercase
    }
    // castling is a special case that will require a separate path for handling
    return current_move;
  }



  return current_move;
}

// TODO - impliment verification of castling
// 1. check if state says castling availability is there for type of castling
// requested
// 2. check if any pieces are in between the king and rook
// 3. check if castling would put king in check in any of those positions the
// king would move across. This should be outsourced to another function TODO
bool validate_castles(char piece, bool is_short_castle, char state_castling_availability[]) {

  return false;
}


// TODO - I can factor this into actually changing the move to always placing a space where
// the move started and placing the piece I find at the board state in the location of the end space
void change_board_state(struct State state, char start_rank, char start_file, char end_rank, char end_file) {
  char piece = state.board[translate_rank(start_rank)][translate_file(start_file)];

  state.board[translate_rank(start_rank)][translate_file(start_file)] = ' ';
  state.board[translate_rank(end_rank)][translate_file(end_file)] = piece;
  return;
}


/*
 * take a parsed move and iterate through it to update game state correctly
 * NOTE: updates State parameter directly and returns nothing
 */
bool make_move(struct State state, struct Move move) {
  if (move.is_castles) {
    bool castle_is_legal = validate_castles(move.piece, move.castles_short, state.castling_availability);
    if (castle_is_legal) {
      if(move.piece == 'k') {
        if (move.castles_short) {
          change_board_state(state,'8','e','8', 'g');
          change_board_state(state,'8','h','8', 'f');
          return true;
        } else {
          change_board_state(state,'8','e','8', 'c');
          change_board_state(state,'8','a','8', 'd');
          return true;
        }
      } else {
        if (move.castles_short) {
          change_board_state(state,'1','e','1', 'g');
          change_board_state(state,'1','h','1', 'f');
          return true;
        } else {
          change_board_state(state,'1','e','1', 'c');
          change_board_state(state,'1','a','1', 'd');
          return true;
        }
      }
    } else {
      fprintf(stderr, "tried to castle with move and castling not allowed, try again!");
      return false;
    }
  }

  return true;
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

  char sample_fen[FEN_MAX] =  "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 1 100";
  char fen_arr[10][FEN_MAX] = {
    "rnbqkbnr/pppp1ppp/8/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2",
    "r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4",
    "r2q1rk1/ppp2ppp/2np1n2/2b1p1B1/2B1P1b1/2NP1N2/PPP2PPP/R2Q1RK1 w - - 4 8",
    "8/5pk1/6p1/7p/7P/6P1/5PK1/8 w - - 0 40",
    "4k3/8/8/3q4/8/8/3Q4/4K3 w - - 0 1",
    "r3k2r/pp3ppp/2n1bn2/3pp3/3P4/2P1BN2/PP3PPP/R3K2R w KQkq - 0 12",
    "2r3k1/5ppp/p3p3/1p6/1P6/P3P1P1/5P1P/2R3K1 b - - 0 28",
    "8/2k5/8/2P5/8/8/4K3/8 w - - 0 1",
    "rnbqkb1r/pp2pppp/3p1n2/8/3NP3/8/PPP2PPP/RNBQKB1R w KQkq - 1 5",
    "6k1/5p2/6p1/8/3R4/6P1/5PK1/3r4 w - - 0 45"
  };


  size_t rows = sizeof(fen_arr) / sizeof(fen_arr[0]);
  for(int i = 0; i < rows; i++) {
    struct State sample_state = build_state_from_fen(fen_arr[i]);
    draw_board(sample_state.board);
    printf("current turn: %c\n", sample_state.current_turn);
    printf("castling availability: %s\n", sample_state.castling_availability);
    printf("en passant targets: %s\n", sample_state.en_passant_target);
    printf("halfmove clock: %d\n", sample_state.halfmove_clock);
    printf("fullmove clock: %d\n", sample_state.fullmove_clock);
  }

  // printf("input FEN is: %s\n", sample_state.fen);
  // printf("output FEN is: %s\n", generate_FEN(sample_state));
  // draw_board(game_state.board);
  // char *fen = generate_FEN(game_state);
  // printf("Game FEN is: %s\n", fen);
  // free(fen);
  return 0;
}
