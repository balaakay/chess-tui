

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
  char suffix; // for check and mate
};
