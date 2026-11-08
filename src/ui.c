

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
