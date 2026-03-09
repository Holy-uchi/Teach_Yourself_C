#include <stdio.h>
#define A
#define C

int main(void) {

#if !defined(A)
  puts("no A");
#elif !defined(B)
  puts("no B");
#elif !defined(C)
  puts("no C");
#else
  puts("with C");

#endif
}
