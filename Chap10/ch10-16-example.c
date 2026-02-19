#include <stdio.h>
int main(int argc, char *argv[]) {
  (void)argv;
  if (argc != 2) {
    fprintf(stderr, "コマンドライン引数は1つ指定してください。%i個指定されています。\n", argc - 1);
  }
}
