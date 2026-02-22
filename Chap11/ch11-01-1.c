#include <stdio.h>

typedef struct {
  int width;
  int height;
  int depth;
} CUBE;

typedef struct {
  char name[32];
  int name_length;
} NAME;

typedef struct {
  double latitude;
  double longitude;
  char *landmark;
} GEOPOINT;
