//  #include <stdio.h>
//
//  int main(void) {
//    char digit[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
//    for (int i = 0; i < 10; i++) {
//      printf("letter=%c, code=%i\n", digit[i], digit[i]);
//    }
//  }

//  #include <stdio.h>
//  int main(void) {
//    char a0 = '\xE3';
//    char a1 = '\x81';
//    char a2 = '\x82';
//    printf("%c%c%c\n", a0, a1, a2);
//  }

//  #include <locale.h>
//  #include <stddef.h>
//  #include <stdint.h>
//  #include <stdio.h>
//
//  int main(void) {
//    setlocale(LC_CTYPE, "ja_JP.utf-8");
//    wchar_t a = L'あ';
//    int16_t a0 = u'あ';
//    int32_t a1 = U'あ';
//    printf("%lc\n", a1);
//  }

#include <ctype.h>
#include <stdio.h>

int main(void) {
  char c[] = {'1', 'a', 'B', ' ', '\n'};
  for (int i = 0; i < 5; i++) {
    printf("%c alnum=%i, alpha=%i, blank=%i, control=%i, digit=%i\n", c[i], isalnum(c[i]),
           isalpha(c[i]), isblank(c[i]), iscntrl(c[i]), isdigit(c[i]));
    if (islower(c[i])) {
      printf("%c => %c\n", c[i], toupper(c[i]));
    } else if (isupper(c[i])) {
      printf("%c => %c\n", c[i], tolower(c[i]));
    }
  }
}
