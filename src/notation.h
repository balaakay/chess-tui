#ifndef NOTATION_H
#define NOTATION_H

#include "game.h"

int translate_rank(char rank);
int translate_file(char file);
char *find_square_name(int rank, int file);
char *generate_FEN(struct State state);
struct State build_state_from_fen(char fen[FEN_MAX]);

#endif
