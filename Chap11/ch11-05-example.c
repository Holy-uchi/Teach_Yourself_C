#include <stdio.h>

typedef struct {
  char name[128];
  int born;
  int died;
} PERSON;

int main(void) {
  PERSON Keitai = {"Keitai", 450, 531};
  PERSON Kagemusha = Keitai;
  Keitai.died = 480;

  printf("%s, %i, %i\n", Keitai.name, Keitai.born, Keitai.died);
  printf("%s, %i, %i\n", Kagemusha.name, Kagemusha.born, Kagemusha.died);
}
