// 第１問
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  unsigned int seven : 3;
  unsigned int heaven : 1;
  unsigned int given : 5;
} SIN;

// 第２問

struct person {
  int age;
  int experienced;
  union {
    char member_id[8];
    char *member_ptr;
  };
};

// 第３問

void print_person(struct person *p, bool use_id) {
  if (use_id) {
    printf("age:%i, experienced:%i, id:%s\n", p->age, p->experienced, p->member_id);
  } else {
    printf("age:%i, experienced:%i, name:%s\n", p->age, p->experienced, p->member_ptr);
  }
}

int main(void) {
  struct person p0 = {
      .age = 24,
      .experienced = 8,
      .member_id = "Tako M",
  };
  print_person(&p0, true);
  struct person p1 = {
      .age = 35,
      .experienced = 8,
      .member_ptr = "Akiyama Jiro",
  };
  print_person(&p1, false);
}
