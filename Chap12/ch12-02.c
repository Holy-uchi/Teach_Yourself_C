#include <stdbool.h>
#include <stdio.h>

static char *ECO_MODE[] = {"OFF", "ON"};
static char *SHIFT[] = {"P", "R", "N", "D", "L"};
static char *SIDE_BREAK[] = {"OFF", "ON"};
static char *WINKER[] = {"左", "中", "右"};
static char *SWITCH_POS[] = {"停止", "電源ON", "エンジンON", "スターター"};

typedef struct {
  unsigned int eco_mode : 1;
  unsigned int shift : 3;
  bool side_break : 1;
  unsigned int engine : 13; // 0 ~ 1024 * 8 - 1
  signed int winker : 2;
  unsigned int switch_pos : 2;
} CAR;

int main(void) {
  int b1[] = {0, 0, 1, 0, 0, 0};
  int b2[] = {1, 3, 0, 1500, 0, 2};
  int b3[] = {0, 2, 0, 800, -1, 2};

  int *b[3] = {b1, b2, b3};

  CAR B[3] = {0};

  for (int i = 0; i < 3; i++) {
    B[i].eco_mode = b[i][0];
    B[i].eco_mode = b[i][0];
    B[i].shift = b[i][1];
    B[i].side_break = b[i][2];
    B[i].engine = b[i][3];
    B[i].winker = b[i][4];
    B[i].switch_pos = b[i][5];
  }

  for (int i = 0; i < 3; i++) {
    CAR car = B[i];
    printf("%i台目\n", i + 1);
    printf("エコモード: %s\nシフト位置: %s\nサイドブレーキ: %s\nエンジン回転数: "
           "%i\nウィンカー位置: %s\nスイッチ: %s\n",
           ECO_MODE[car.eco_mode], SHIFT[car.shift], SIDE_BREAK[car.side_break], car.engine,
           WINKER[car.winker + 1], SWITCH_POS[car.switch_pos]

    );
  }
}
