//  #include <_stdio.h>
//  #include <stdio.h>
//
//  int main(void) {
//    int ch;
//    while ((ch = getchar()) != EOF) {
//      putchar(ch);
//    }
//  }

// fgets
//
//  #include <stdio.h>
//  int main(void) {
//    char buffer[10];
//    while (fgets(buffer, 10, stdin) != NULL) {
//      puts(buffer);
//    }
//  }

// printfの書式設定
//  #include <stdio.h>
//  int main(void) {
//    printf("%4i\n", 10000);
//    printf("%3i\n", 10000);
//    printf("%4.3i\n", 10000);
//    printf("%4.1i\n", 10000);
//    printf("%4i\n", 10);
//    printf("%3i\n", 10);
//    printf("%04i\n", 10);
//    printf("%4.3i\n", 10);
//    printf("%4.1i\n", 10);
//  }

//  #include <stdio.h>
//  int main(void) {
//    printf("%4f\n", 1.00001);
//    printf("%3f\n", 1.00001);
//    printf("%4.3f\n", 1.00001);
//    printf("%4.1f\n", 1.00001);
//    printf("%8.1f\n", 1.00001);
//    printf("%08.1f\n", 1.00001);
//  }

//  #include <stdio.h>
//
//  int main(void) {
//    char *p = "0123456789";
//    printf("%12s\n", p);
//    printf("%12.3s\n", p);
//    printf("%.3s\n", p);
//    printf("%8.8s\n", p);
//  }

//  #include <stdio.h>
//  int main(void) {
//    puts("012345678901234567890123456789");
//    printf("%-10i%-10f%-10s\n", 1234, 1.0, "01234");
//  }

//  #include <stdio.h>
//  int main(void) {
//    printf("%*.3s\n", 5, "abc");
//    printf("%5.*s\n", 3, "abc");
//    printf("%*.*s\n", 5, 3, "abc");
//  }

//  #include <stdio.h>
//  int main(void) {
//    int n;
//    char c;
//    long l;
//    double d;
//    char str[5];
//    int ret = scanf("%i,%*c,%c,%li,%lf,%4s", &n, &c, &l, &d, str);
//    printf("read %i valued\n", ret);
//    printf("n=%i, c=%c, l=%li, d=%f, str=%s\n", n, c, l, d, str);
//  }

//  #include <stdio.h>
//
//  int main(void) {
//    char tmp[15];
//    int ret = scanf("%14[abcdef]", tmp);
//    printf("read %i valued\n", ret);
//    if (ret) {
//      puts(tmp);
//    }
//  }

#include <stdio.h>
int main(void) {
  char str[80];
  snprintf(str, 80, "%i, %f, %.10s", 3, 3.0, "abcdefghijklm");
  puts(str);
  int n;
  double d;
  char str2[80];

  int ret = sscanf(str, "%i, %lf, %10s", &n, &d, str2);
  printf("ret=%i, %i, %f, %.10s\n", ret, n, d, str2);

  printf("%#X\n", 32);
}
