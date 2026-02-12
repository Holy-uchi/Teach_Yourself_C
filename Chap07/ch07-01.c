// 第２問
//  #include <stdio.h>
//
//  int main(void) {
//    char alphabet[] = {
//        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
//        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
//    };
//    for (int i = 0; i < 26; i++) {
//      printf("%c\n", alphabet[i]);
//    }
//  }

// 第３問
//  #include <locale.h>
//  #include <stddef.h>
//  #include <stdio.h>
//  int main(void) {
//    setlocale(LC_CTYPE, "ja_JP.utf-8");
//    wchar_t n[] = {
//        L'\x306A', L'\x306B', L'\x306C', L'\x306D', L'\x306E',
//    };
//
//    for (int i = 0; i < 5; i++) {
//      printf("%lc\n", n[i]);
//    }
//  }

// 第３問-やり直し
//  #include <locale.h>
//  #include <stddef.h>
//  #include <stdio.h>
//  #include <wchar.h>
//
//  int main(void) {
//    setlocale(LC_CTYPE, "ja_JP");
//    for (wchar_t c = L'な'; c <= L'の'; c++) {
//      printf("%lc\n", c);
//    }
//  }

// 第４問
#include <_ctype.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  (void)argc;
  if (argc != 2) {
    puts("give an argument");
    return 1;
  }
  char ch = argv[1][0];
  if (isupper(ch)) {
    printf("%c\n", tolower(ch));
  } else if (isdigit(ch)) {
    printf("%i\n", ch - '0' + 3);
  } else {
    puts("英大文字または数字を入力してください。");
  }
}
