//  第１問
//  1. char three = '3';
//  2. wchar_t a = L'あ';
//  3. char hello[] = "Hello";
//  4. char hello[] = { 'H', 'e', 'l', 'l', 'o', '\0',};
//
//  第２問
//  a. 1
//
//  第３問
//  #include <stdio.h>
//  #include <string.h>
//
//  int main(void) {
//    char x[] = "Hello world!";
//    size_t len = strlen(x) + 1;
//    char y[len];
//    strcpy(y, x);
//    int diff = strcmp(x, y);
//    if (diff == 0) {
//      puts("match!");
//    }
//  }
//
//
//  第４問
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    return 0;
  }
  size_t len = strlen(argv[1]);
  char buff[len + 1];
  for (int i = 0; i < (int)len; i++) {
    buff[i] = argv[1][len - 1 - i];
  }
  buff[len] = '\0';

  puts(buff);
}
