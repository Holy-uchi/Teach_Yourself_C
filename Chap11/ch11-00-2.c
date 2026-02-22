#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define RESULT_FILE "data/Chap11/last-result.data"

bool filesize(FILE *fp, long *out_size);
bool ensure_valid_file_size(FILE *fp, size_t expected);
bool save_result(int32_t result);
bool get_last_result(int32_t *last_result);

int main(int argc, char *argv[]) {
  int32_t base = 0;
  int32_t last_result;
  if (argc >= 2) {
    base = (int32_t)atoi(argv[1]);
  } else if (get_last_result(&last_result)) {
    // コマンドライン引数なし & last_result取得成功
    base = last_result;
  } else {
    // コマンドライン引数なし & last_result取得失敗
    base = 0;
  }

  int32_t current_result = base + 10;
  printf("%i\n", current_result);

  if (!save_result(current_result)) {
    return 2;
  }

  return 0;
}

bool get_last_result(int32_t *last_result) {
  FILE *fp = fopen(RESULT_FILE, "rb");
  if (!fp) {
    if (errno == ENOENT) {
      // 初回: ファイルが無いのは正常
      return false;
    }
    perror("fopen (get_last_result)");
    return false;
  }

  if (!ensure_valid_file_size(fp, sizeof(int32_t))) {
    fclose(fp);
    return false;
  }

  rewind(fp);

  int32_t last_result_local;
  size_t n = fread(&last_result_local, sizeof(last_result_local), 1, fp);
  if (ferror(fp)) {
    perror("fread (get_last_result)");
    fclose(fp);
    return false;
  } else if (n != 1) {
    fprintf(stderr, "unexpected error (get_last_result)\n");
    fclose(fp);
    return false;
  }

  if (fclose(fp)) {
    perror("fclose (get_last_result)");
    return false;
  }

  *last_result = last_result_local;
  return true;
}

bool ensure_valid_file_size(FILE *fp, size_t expected) {
  long size = 0;
  if (!filesize(fp, &size)) {
    return false;
  }

  if ((size_t)size != expected) {
    fprintf(stderr, "file format invalid. expected: %zu got: %zu (ensure_valid_file_size)\n",
            expected, (size_t)size);
    return false;
  }

  return true;
}

bool save_result(int32_t result) {
  FILE *fp = fopen(RESULT_FILE, "wb");
  if (!fp) {
    perror("fopen (save_result)");
    return false;
  }

  size_t n = fwrite(&result, sizeof(result), 1, fp);
  if (ferror(fp)) {
    perror("fwrite (save_result)");
    fclose(fp);
    return false;
  } else if (n != 1) {
    fprintf(stderr, "unexpected error (save_result)\n");
    fclose(fp);
    return false;
  }

  if (fclose(fp)) {
    perror("fclose (save_result)");
    return false;
  }

  return true;
}

bool filesize(FILE *fp, long *out_size) {
  long cur = ftell(fp);
  if (cur == -1L) {
    perror("ftell");
    return false;
  }

  if (fseek(fp, 0, SEEK_END) != 0) {
    perror("fseek");
    return false;
  }

  long size = ftell(fp);
  if (size == -1L) {
    perror("ftell");
    return false;
  }

  if (fseek(fp, cur, SEEK_SET) != 0) {
    perror("fseek");
    return false;
  }

  *out_size = size;
  return true;
}
