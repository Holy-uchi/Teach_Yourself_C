// 第１問

#include <stdio.h>

int main(void) {
  char a[] = "This is a string.";
  int len = 0;
  while (a[len] != '\0') {
    len++;
  }
  printf("%i\n", len);
}

// 第２問
// 1. 10 byte
// 2. 20 byte
// 3. 40 byte
// 4. 20 byte
// ↑間違い。
