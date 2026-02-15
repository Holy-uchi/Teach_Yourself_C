// 第１問
//  #include <stdio.h>
//  #include <stdlib.h>
//
//  void print_count(void) {
//    static int current = 1;
//    printf("%i\n", current++);
//  }
//
//  int main(int argc, char *argv[]) {
//    if (argc != 2) {
//      return 1;
//    }
//    for (int i = 0; i < atoi(argv[1]); i++) {
//      print_count();
//    }
//  }

// 第３問
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void func(char *method, ...) {
  va_list ap;
  va_start(ap, method);
  int sum = 0;
  int arg;
  int argc = 0;
  bool first = true;

  while ((arg = va_arg(ap, int)) != -1) {
    sum += arg;
    argc++;
    if (first) {
      printf("%i", arg);
      first = false;
    } else {
      printf(", %i", arg);
    }
  }
  va_end(ap);

  if (strcmp(method, "合計") == 0) {
    printf("の合計は%iです。\n", sum);
  } else if (strcmp(method, "平均") == 0) {
    printf("の平均は%fです。\n", (double)sum / (double)argc);
  }
}

int main(void) {
  func("合計", 1, 2, 3, -1);
  func("平均", 1, 2, 3, -1);
  func("合計", 1, 2, 3, 4, 5, -1);
  func("平均", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, -1);
}
