#include <stddef.h>
#include <stdio.h>
#define BYTE_SIZE 1000
#define FILE_NAME "data/Chap11/1K.data"

int q1(void);
int q2(void);
int q3(void);

int main(void) {
  return !q1() && !q2() && !q3(); // それぞれ成功したら0を返す
}

// 第１問
int q1(void) {
  FILE *fp = fopen(FILE_NAME, "wb");
  if (!fp) {
    perror("fopen");
    return 2;
  }
  unsigned char bytes[BYTE_SIZE] = {0};
  // for (int i = 0; i < BYTE_SIZE; i++) {
  //   bytes[i] = 0;
  // }

  size_t n = fwrite(bytes, sizeof(bytes[0]), BYTE_SIZE, fp);
  if (ferror(fp)) {
    perror("fwrite");
    fclose(fp);
    return 2;
  } else if (n != BYTE_SIZE) {
    fprintf(stderr, "unexpected error");
    fclose(fp);
    return 2;
  }

  if (fclose(fp)) {
    perror("fclose");
    return 2;
  }

  return 0;
}

// 第２問
int q2(void) {
  FILE *fp = fopen(FILE_NAME, "rb+");
  if (!fp) {
    perror("fopen");
    return 2;
  }

  for (int i = 1; i * 32 < BYTE_SIZE; i++) {
    if (fseek(fp, i * 32 - 1, SEEK_SET) != 0) {
      perror("fseek");
      return 2;
    }
    if (fputc('A', fp) == EOF) {
      perror("fputc");
      return 2;
    }
  }

  if (fclose(fp)) {
    perror("fclose");
    return 2;
  }

  return 0;
}

// 第３問
int q3(void) {
  FILE *fp = fopen(FILE_NAME, "rb");
  if (!fp) {
    perror("fopen");
    return 2;
  }

  unsigned char buff[BYTE_SIZE];
  size_t n = fread(buff, sizeof(buff[0]), BYTE_SIZE, fp);

  if (ferror(fp)) {
    perror("fread");
    return 2;
  } else if (n != (size_t)BYTE_SIZE) {
    fprintf(stderr, "read data too short");
    return 2;
  }

  printf("0--1--2--3--4--5--6--7--8--9--A--B--C--D--E--F-\n");
  int count = 0;
  for (int i = 0; i < BYTE_SIZE; i++) {
    unsigned char uc = buff[i];
    printf("%02X", uc);
    count++;
    if (count % 16 == 0) {
      printf("\n");
    } else {
      printf(" ");
    }
  }

  if (fclose(fp)) {
    perror("fclose");
    return 2;
  }

  return 0;
}
