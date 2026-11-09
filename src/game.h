#ifndef GAME_H
#define GAME_H

#include "board.h"

#define FEN_MAX 95

struct State {
  char board[ROWS][COLS];
  char current_turn;
  char castling_availability[5];
  char en_passant_target[3];
  int halfmove_clock;
  int fullmove_clock;
  char fen[FEN_MAX];
};

#endif
