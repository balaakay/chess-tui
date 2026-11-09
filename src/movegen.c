

#include <stdlib.h>
#include <stdbool.h>

#include "movegen.h"
#include "game.h"
#include "notation.h"

static _Noreturn void unreachable(void) {
  abort();
}

/* This function is for finding the actual squares that en_passant_target could
 * identify in the fen string. 
 * NOTE: this function assumes the move passed is a pawn move without capture */
char* check_and_find_en_passant(struct State *state, char *pawn_move) {
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
    if(state->board[rank-1][file] == 'p') {
      return "-";
    } else {
      return find_square_name(rank-1, file);
    }
  }

  // check white pawn move
  if(rank == conditional_white_rank) {
    if(state->board[rank+1][file] == 'P') {
      return "-";
    } else {
      return find_square_name(rank + 1, file);
    }
  }
  unreachable();
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
