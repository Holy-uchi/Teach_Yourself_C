#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#define INITIAL_CAP 32
#define GROWTH_CHUNK 32

int read_line(FILE *fp, char **out_lin);

//  int read_line(FILE *fp, char **out_line) {
//    if (!fp || !out_line || *out_line != NULL) {
//      return EINVAL;
//    }
//
//    char *tmp = malloc(INITIAL_CAP);
//    if (!tmp) {
//      return ENOMEM;
//    }
//    size_t last = 0;
//    size_t csize = INITIAL_CAP;
//    int ch;
//    while ((ch = fgetc(fp)) != EOF) {
//      if (last + 1 >= csize) {
//        char *ntmp = realloc(tmp, csize + GROWTH_CHUNK);
//        if (!ntmp) {
//          free(tmp);
//          return ENOMEM;
//        }
//        tmp = ntmp;
//        csize += GROWTH_CHUNK;
//      }
//      tmp[last++] = (char)ch;
//      if (ch == '\n') {
//        break;
//      }
//    }
//
//    if (ferror(fp)) {
//      free(tmp);
//      return EIO; // これなんの定数がベスト？
//    }
//
//    if (last == 0 && ch == EOF) {
//      free(tmp);
//      return EOF; // もう読むものがない
//    }
//
//    tmp[last] = '\0';
//    *out_line = tmp;
//    return 0;
//  }

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "give a file name\n");
    return EINVAL;
  }

  FILE *fp = fopen(argv[1], "rb");
  if (!fp) {
    perror("fopen");
    return errno; // なんの定数がいい？
  }

  char *buff = NULL;
  int rc;
  while ((rc = read_line(fp, &buff)) == 0) {
    puts(buff);
    free(buff);
    buff = NULL;
  }

  fclose(fp);

  if (rc != EOF) {
    fprintf(stderr, "read_line failed %d\n", rc);
    return rc;
  }

  return 0;
}

int read_line(FILE *fp, char **out_line) {
  if (!fp || !out_line || *out_line != NULL) {
    return EINVAL;
  }

  size_t last = 0;
  size_t cap = INITIAL_CAP;
  char *tmp = malloc(INITIAL_CAP);
  if (!tmp) {
    return ENOMEM;
  }
  int ch;
  while ((ch = fgetc(fp)) != EOF) {
    if (last + 1 >= cap) {
      char *ntmp = realloc(tmp, cap * 2);
      if (!ntmp) {
        free(tmp);
        return ENOMEM;
      }
      cap *= 2;
      tmp = ntmp;
    }

    tmp[last++] = (char)ch;

    if (ch == '\n') {
      break;
    }
  }

  if (ferror(fp)) {
    free(tmp);
    return EIO;
  }

  if (last == 0 && ch == EOF) {
    free(tmp);
    return EOF;
  }

  tmp[last] = '\0';
  *out_line = tmp;
  return 0;
}
