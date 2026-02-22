#include <stdio.h>

typedef struct {
  int x;
  int y;
} POINT;

void move_to(POINT *p, int x, int y) {
  p->x += x;
  p->y += y;
}

int main(void) {
  POINT p = {1, 2};
  printf("%i, %i\n", p.x, p.y);
  move_to(&p, 10, -10);
  printf("%i, %i\n", p.x, p.y);
}
