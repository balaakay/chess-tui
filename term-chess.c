#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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


// TODO
char* generate_board_state_for_FEN(char m[ROWS][COLS]) {

  char *s = "";
  return s;
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



// parse the fen string and create a board state object with all the right states
struct State build_state_from_fen(char fen[95]) {
  struct State new_state;
  strcpy(new_state.fen, fen);
  // printf("new_state.fen sizeof strlen: %d\n", (int) strlen(new_state.fen));
  // printf("build_state_from_fen strncpy result: %s\n", new_state.fen);

  char *token = strtok(fen, " "); // split fen by space, first run gets board string
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
  // printf("board string: %s\n", board_string);
  build_board_array(board_string, new_state.board);

  //
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

  char sample_fen[95] =  "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 1 100";
  char fen_arr[10][95] = {
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
