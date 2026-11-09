#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "move.h"
#include "game.h"
#include "notation.h"
#include "movegen.h"

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
