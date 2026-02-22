#include <stdio.h>

typedef struct {
  int x;
  int y;
} POINT;

int main(void) {
  POINT left_top = {.y = 10};
  POINT left_bottom = {0};
  POINT right_top = {.x = 10, .y = 10};
  POINT right_bottom = {10, .y = 0};

  printf("x=%i, y=%i\n", left_top.x, left_top.y);
  printf("x=%i, y=%i\n", left_bottom.x, left_bottom.y);
  printf("x=%i, y=%i\n", right_top.x, right_top.y);
  printf("x=%i, y=%i\n", right_bottom.x, right_bottom.y);
}
