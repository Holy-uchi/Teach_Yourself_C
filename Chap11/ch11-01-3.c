#include <stdio.h>

typedef struct {
  int x;
  int y;
} POINT;

POINT move_to(POINT p, int x, int y) {
  printf("%p\n", (void *)&p);
  p.x += x;
  p.y += y;
  return p;
}

int main(void) {
  POINT p = {1, 2};
  printf("%p\n", (void *)&p);
  printf("%i, %i\n", p.x, p.y);
  POINT pp = move_to(p, 10, -10);
  printf("%p\n", (void *)&pp);
  printf("%i, %i\n", pp.x, pp.y);
}
