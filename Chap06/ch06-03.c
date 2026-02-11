#include <stdio.h>

int main(void) {
  int a0[][2][3] = {{
                        {1, 2, 3},
                        {4, 5, 6},
                    },
                    {{7, 8, 9}, {10, 11, 12}}};

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-braces"
#endif
  int a1[][2][3] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

  for (int x = 0; x < 2; x++) {
    for (int y = 0; y < 2; y++) {
      for (int z = 0; z < 4; z++) {
        // Intentionally out-of-bounds (z==3) to demonstrate UB.
        // Run with AddressSanitizer to catch it:
        //   clang -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer file.c && ./a.out
        int v0 = a0[x][y][z];
        int v1 = a1[x][y][z];
        printf("a0(%i, %i, %i) = %i\n", x, y, z, v0);
        printf("a1(%i, %i, %i) = %i\n", x, y, z, v1);
      }
    }
  }
}
