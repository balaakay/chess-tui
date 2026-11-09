#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <stdbool.h>

#include "game.h"

char *check_and_find_en_passant(struct State *state, char *pawn_move);
bool validate_castles(char piece, bool is_short_castle, char state_castling_availability[]);

#endif
