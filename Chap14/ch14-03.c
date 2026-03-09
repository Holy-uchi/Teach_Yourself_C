#include <stdio.h>
#define DEF_VAR(arg, x) int arg##x = x + 1;
#define DEF_VAR10(arg, y) int arg##y = 10 * (y + 1);

#define VP(a, b, c, d, e, f) printf("%i, %i, %i, %i\n", a, b, c, (d + e + f));
#define VP2(tmpl, ...) printf(tmpl, __VA_ARGS__);
#define MOVE_PROPERTY(x, y, z) x.z = y.z;
#define SAVE_STRUCT(struct_type, name) struct_type name##_save = name;

int main(void) {
  DEF_VAR(x, 0)
  DEF_VAR(x, 1)
  DEF_VAR(x, 2)
  DEF_VAR(x, 3)
  DEF_VAR(x, 4)
  DEF_VAR10(y, 0)
  DEF_VAR10(y, 1)
  DEF_VAR10(y, 2)
  DEF_VAR10(y, 3)
  DEF_VAR10(y, 4)
  (void)x0;
  (void)x1;
  (void)x2;
  (void)x3;
  (void)x4;
  (void)y0;
  (void)y1;
  (void)y2;
  (void)y3;
  (void)y4;

  printf("%i\n", x0);
}
