#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int x;
  int y;
} X;
// 第1問
//  X *create_X(int x, int y) {
//    X *xp = malloc(sizeof(X));
//    xp->x = x;
//    xp->y = y;
//    return xp;
//  }

// 第2問
X create_X(int x, int y) {
  X xs = {x, y};
  return xs;
}

#define MAX_XS 8
int main(void) {
  X xs[MAX_XS];
  for (int i = 0; i < MAX_XS; i++) {
    xs[i] = create_X(i, i);
  }
  for (int i = 0; i < MAX_XS; i++) {
    printf("%i, %i\n", xs[i].x, xs[i].y);
  }
}
