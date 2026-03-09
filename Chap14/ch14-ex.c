#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PRINT_VEC(vec, size, type)                                                                 \
  for (long i = 0; i < (long)(size); i++) {                                                        \
    printf(#vec "[%li]=%" #type "\n", i, vec[i]);                                                  \
  }
#define PRINT_ARG(var, sp) printf("%s=" # sp "\n", #var, var)


static char* month_vec[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

int main(int argc, char *argv[]) { 
  char *date = __DATE__;
  char *time = __TIME__;
  int year;
  char month[4];
  int date2;
  memcpy(month, date, 3);
  month[3] = '\0';
  int n = 0;
  while (strcmp(month_vec[n], month)) {
    n++;
    if (n > 11) {
      return 1;
    }
  }
  sscanf(date, "%*s %d %d", &date2, &year);
  printf("%i/%02i/%02i %s\n", year, n+1, date2, time );

  PRINT_ARG(argc, %i);
  PRINT_VEC(argv, argc, s); 
}
