// 第１問
// b, c, f

// 第２問
// 1. '\0'
// 2. '\t'
// 3. '\n'
// 4. '\r'
// 5. '\x1B'

// 第３問
//  #include <stdio.h>
//  int main(void) {
//    char hello[] = {
//        'H', 'e', 'l', 'l', 'o', '!',
//    };
//    puts(hello);
//  }

// 第４問
//  #include <ctype.h>
//  #include <stdio.h>
//  #include <string.h>
//
//  int main(int argc, char *argv[]) {
//
//    if (argc != 2) {
//      puts("give an argument.");
//      return 1;
//    }
//
//    if (strlen(argv[1]) != 1) {
//      puts("give a charactor");
//      return 1;
//    }
//
//    char ch = argv[1][0];
//    if (isalpha(ch)) {
//      puts("A");
//    } else if (isdigit(ch)) {
//      puts("B");
//    } else {
//      puts("C");
//    }
//  }

// 第５問
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[]) {
  if (argc != 3) {
    puts("give 2 arguments.");
    return 1;
  }
  size_t first_len = strlen(argv[1]);
  size_t second_len = strlen(argv[2]);
  char first_low[first_len + 1];
  char second_low[second_len + 1];
  strcpy(first_low, argv[1]);
  strcpy(second_low, argv[2]);

  for (int i = 0; i < (int)strlen(first_low); i++) {
    char ch = first_low[i];
    first_low[i] = tolower(ch);
  }

  for (int i = 0; i < (int)strlen(second_low); i++) {
    char ch = second_low[i];
    second_low[i] = tolower(ch);
  }

  if (strcmp(first_low, second_low) > 0) {
    puts("1");
  } else if (strcmp(first_low, second_low) < 0) {
    puts("-1");
  } else {
    puts("0");
  }
}
