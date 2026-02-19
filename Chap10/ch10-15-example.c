#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
int main(void) {
  int32_t int_data;
  int32_t iarray_data[5];
  uint8_t binary_data[80];
  char char_data[16];
  int64_t long_data;
  int64_t larray_data[5];

  FILE *fp = fopen("savedata.bin", "rb");
  if (fp) {
    size_t sz = fread(&int_data, sizeof(int32_t), 1, fp);
    printf("load int32_t data:%zu counts\n", sz);

    sz = fread(iarray_data, sizeof(int32_t), 5, fp);
    printf("load int32_t[5] data:%zu coutns\n", sz);

    sz = fread(binary_data, sizeof(uint8_t), 80, fp);
    printf("load uint8_t[80] data:%zu counts\n", sz);

    sz = fread(char_data, sizeof(char), 16, fp);
    printf("load char[16] data:%zu coutns\n", sz);

    sz = fread(&long_data, sizeof(int64_t), 1, fp);
    printf("load int64_t data:%zu counts\n", sz);

    sz = fread(larray_data, sizeof(int64_t), 5, fp);
    printf("load int64_t[5] data:%zu coutns\n", sz);

    printf("int_data = %i\n", int_data);
    printf("long_data = %lli\n", long_data);
    for (int i = 0; i < 5; i++) {
      printf("iarray_data[%i] = %i, larray_data[%i] = %lli\n", i, iarray_data[i], i,
             larray_data[i]);
    }
    printf("char_data = %s\n", char_data);
    for (int i = 0; i < 80; i += 8) {
      printf("%02x %02x %02x %02x %02x %02x %02x %02x\n", binary_data[i], binary_data[i + 1],
             binary_data[i + 2], binary_data[i + 3], binary_data[i + 4], binary_data[i + 5],
             binary_data[i + 6], binary_data[i + 7]);
    }
  } else {
    fp = fopen("savedata.bin", "wb");

    int_data = 654321;
    long_data = 987654321;
    for (int i = 0; i < 5; i++) {
      iarray_data[i] = 12 * i;
      larray_data[i] = 123 * i;
    }
    strcpy(char_data, "hello world!");
    for (int i = 0; i < 80; i++) {
      binary_data[i] = (uint8_t)i;
    }
    size_t sz = fwrite(&int_data, sizeof(int32_t), 1, fp);
    printf("save int32_t data:%zu counts\n", sz);

    sz = fwrite(iarray_data, sizeof(int32_t), 5, fp);
    printf("save int32_t[5] data:%zu counts\n", sz);

    sz = fwrite(binary_data, sizeof(uint8_t), 80, fp);
    printf("save uint8_t[80] data:%zu counts\n", sz);

    sz = fwrite(char_data, sizeof(char), 16, fp);
    printf("save char[16] data:%zu counts\n", sz);

    sz = fwrite(&long_data, sizeof(int64_t), 1, fp);
    printf("save int64_t data:%zu counts\n", sz);

    sz = fwrite(larray_data, sizeof(int64_t), 5, fp);
    printf("save int64_t[5] data:%zu counts\n", sz);
  }

  fclose(fp);
}
