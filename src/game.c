
struct State {
  char board[ROWS][COLS];
  char current_turn;
  char castling_availability[4];
  char en_passant_target[2];
  int halfmove_clock;
  int fullmove_clock;
  char fen[FEN_MAX];
};
