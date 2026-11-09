#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>

#include "board.h"
#include "game.h"
#include "notation.h"
#include "move.h"
#include "movegen.h"
#include "ui.h"
// the columns that I want to reference
// #define a 0
// #define b 1
// #define c 2
// #define d 3
// #define e 4
// #define f 5
// #define g 6
// #define h 7




void listen() {
  char move[10];
  if(fgets(move, sizeof(move), stdin) != NULL) {
    printf("move is %s", move);
  }
  return;
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
