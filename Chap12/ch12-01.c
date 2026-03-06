typedef struct {
  int account;
  union {
    char name[32];
    struct {
      char first_name[16];
      char family_name[16];
    };
  };
  int age;
  char address[64];
} PERSON;
