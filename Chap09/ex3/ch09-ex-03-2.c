#include <stddef.h>
#include <stdio.h>
#include <string.h>

char *create_message(char *buff, size_t nbuff, const char *name, const char *greeting) {
  if (!buff || nbuff == 0) {
    return buff;
  }
  buff[0] = '\0';
  strncat(buff, greeting, nbuff - 1);

  size_t used = strlen(buff);
  if (used + 1 < nbuff) {
    strncat(buff, name, nbuff - (used + 1));
  }
  return buff;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    return 1;
  }

  size_t max_len = 256;
  char buff1[max_len];
  char buff2[max_len];
  puts(create_message(buff1, max_len, argv[1], "hello "));
  puts(create_message(buff2, max_len, argv[1], "bye "));
}
