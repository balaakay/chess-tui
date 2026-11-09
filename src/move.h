#ifndef MOVE_H
#define MOVE_H

#include <stdbool.h>

#include "game.h"

struct Move {
  char piece;
  char starting_rank;
  char starting_file;
  bool captures_piece;
  char target_rank;
  char target_file;
  bool is_castles;
  bool castles_short;
  bool is_promotion;
  char promotion_to;
  char suffix;
};

struct Move parse_move(char *move, char turn);
void change_board_state(struct State *state, char start_rank, char start_file, char end_rank, char end_file);
bool make_move(struct State state, struct Move move);

#endif
