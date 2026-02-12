// 第１問

//  #include <stdio.h>
//  #include <string.h>
//  #define MAX_STR_LEN 4
//  int main(int argc, char *argv[]) {
//    char buff[MAX_STR_LEN + 1];
//    for (int i = 1; i < argc; i++) {
//      strncpy(buff, argv[i], MAX_STR_LEN);
//      buff[MAX_STR_LEN] = '\0';
//      puts(buff);
//    }
//  }

// 第２問
//  #include <stdio.h>
//  #include <string.h>
//
//  int main(int argc, char *argv[]) {
//    if (argc == 3) {
//      if (strcmp(argv[1], argv[2]) > 0) {
//        puts(argv[1]);
//      } else if (strcmp(argv[1], argv[2]) < 0) {
//        puts(argv[2]);
//      } else {
//        puts("same!");
//      }
//    }
//  }

// 第３問
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    return 1;
  }

  size_t total_len = 0;
  for (int i = 1; i < argc; i++) {
    total_len += strlen(argv[i]);
  }
  char buff[total_len + 1];
  buff[0] = '\0';
  for (int i = 1; i < argc; i++) {
    strcat(buff, argv[i]);
  }

  // 全部小文字にする
  for (int i = 0; i < (int)total_len; i++) {
    char ch = buff[i];
    if (isupper(ch)) {
      buff[i] = tolower(ch);
    }
  }
  puts(buff);
}
