#include <stdio.h>

#include "board.h"

void build_board_array(char *board_string, char board[ROWS][COLS]) {
  int string_index = 0;
  for(int i = 0; i < ROWS; i++) {
    for(int j = 0; j < COLS; j++) {
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
