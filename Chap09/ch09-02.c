// 第１問 - 1
//  #include <stdio.h>
//  int add(int x, int y) { return x + y; }
//  int main(void) { printf("%i\n", add(1, 2)); }

// 第１問 - 2
#include <stdio.h>
//
int ping(int, int);
int pong(int, int);
//
int main(void) { printf("%i\n", ping(10, 20)); }
//
int ping(int x, int y) { return pong(x, y - 1); }
//
int pong(int x, int y) {
  if (y == 0) {
    return x;
  }
  return ping(x * y, y);
}

// 第２問
typedef void (*RETURN_FUNC)(void);

RETURN_FUNC vfunc(void);
