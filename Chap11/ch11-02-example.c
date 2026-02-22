#include <stdio.h>

typedef struct {
  int x;
  int y;
} POINT;

void destroyer(POINT point) {
  point.x = -1;
  point.y = -1;
}

int main(void) {
  POINT p = {1, 2};
  printf("%i, %i\n", p.x, p.y);
  destroyer(p);
  printf("%i, %i\n", p.x, p.y);
}
