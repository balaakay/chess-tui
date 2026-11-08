

/**
 * @brief translate rank from game rank to array index
 * @param rank the rank looking to be translated to array syntax
 * @return the array index of the rank
 */
int translate_rank(char rank) {
  switch (rank) {
    case '8':
      return 0;
    case '7':
      return 1;
    case '6':
      return 2;
    case '5':
      return 3;
    case '4':
      return 4;
    case '3':
      return 5;
    case '2':
      return 6;
    case '1':
      return 7;
    default:
      fprintf(stderr,
              "error translating rank of a move from char to the corresponding "
              "numerical rank -> %c",
              rank);
      abort();
  }
}


int translate_file(char file) {
  switch (file) {
    case 'a':
      return 0;
    case 'b':
      return 1;
    case 'c':
      return 2;
    case 'd':
      return 3;
    case 'e':
      return 4;
    case 'f':
      return 5;
    case 'g':
      return 6;
    case 'h':
      return 7;
    default:
      fprintf(stderr,
              "error translating file of a move from char to the corresponding "
              "numerical file -> %c",
              file);
      abort();
  }
}

// 
char* find_square_name(int rank, int file) {
  char *square_name = "";
  // rank
  switch (rank) {
    case '0':
      square_name[0] = '8';
      break;
    case '1':
      square_name[0] = '7';
      break;
    case '2':
      square_name[0] = '6';
      break;
    case '3':
      square_name[0] = '5';
      break;
    case '4':
      square_name[0] = '4';
      break;
    case '5':
      square_name[0] = '3';
      break;
    case '6':
      square_name[0] = '2';
      break;
    case '7':
      square_name[0] = '1';
      break;
  }

  switch (file) {
    case '0':
      square_name[1] = 'a';
      break;
    case '1':
      square_name[1] = 'b';
      break;
    case '2':
      square_name[1] = 'c';
      break;
    case '3':
      square_name[1] = 'd';
      break;
    case '4':
      square_name[1] = 'e';
      break;
    case '5':
      square_name[1] = 'f';
      break;
    case '6':
      square_name[1] = 'g';
      break;
    case '7':
      square_name[1] = 'h';
      break;
  }
  return square_name;
}
