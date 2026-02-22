#include <stdio.h>

typedef struct {
  int x;
  int y;
} XY_COORDINATE;

typedef struct {
  XY_COORDINATE xy;
  int z;
} THREE_D_COORDINATE;

int main(void) {
  THREE_D_COORDINATE t = {{11, 12}, 13};
  printf("x=%i, y=%i, z=%i\n", t.xy.x, t.xy.y, t.z);
}
