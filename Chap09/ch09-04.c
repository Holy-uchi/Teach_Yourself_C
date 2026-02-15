// 第１問
//  #include <stdio.h>
//  #include <stdlib.h>
//  int fact1(int n) {
//    if (n == 0) {
//      return 1;
//    }
//    return n * fact1(n - 1);
//  }
//
//  int fact2(int n) {
//    int result = 1;
//    for (int i = 1; i <= n; i++) {
//      result *= i;
//    }
//    return result;
//  }
//
//  int main(int argc, char *argv[]) {
//    if (argc != 2) {
//      return 1;
//    }
//    int n = atoi(argv[1]);
//    printf("%i\n", fact1(n));
//    printf("%i\n", fact2(n));
//  }
//
//  第２問
//
//  #include <stddef.h>
//  #include <stdio.h>
//  #include <string.h>
//  #define HELLO "hello "
//
//  char *create_hello(char *buff, size_t nbuff, const char *name);
//
//  int main(int argc, char *argv[]) {
//    if (argc != 2) {
//      return 1;
//    }
//
//    size_t max_size = 256;
//    char buff[max_size];
//    create_hello(buff, max_size, argv[1]);
//
//    puts(buff);
//  }
//
//  char *create_hello(char *buff, size_t nbuff, const char *name) {
//    if (!buff || nbuff == 0) {
//      return buff;
//    }
//    buff[0] = '\0';
//    strncat(buff, HELLO, nbuff - 1);
//
//    size_t used = strlen(buff);
//    if (used < nbuff - 1) {
//      strncat(buff, name, (nbuff - 1) - used);
//    }
//    return buff;
//  }

#include <stdio.h>
#include <stdlib.h>
long long fact(long long n) {
  static long long last = 0;

  if (!n) {
    return 1;
  }

  long long current = n * fact(n - 1);
  if (!current) {
    return 0;
  } else if (current < last) {
    printf("failed at x = %lli\n", n);
    return 0;
  } else {
    last = current;
    return current;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    return 1;
  }
  int n = atoi(argv[1]);
  long long result = fact(n);
  if (result == -1) {
    printf("failed at x = %i\n", n);
    puts("0");
    return 1;
  }
  printf("%lli\n", result);
}
