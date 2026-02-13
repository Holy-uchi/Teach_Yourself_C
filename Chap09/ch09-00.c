#include <stdio.h>

int (*fp)(int, int);
void (*fp2)(char *);
int (*fp3)(int (*)(int));

int main(void) { return 0; }
