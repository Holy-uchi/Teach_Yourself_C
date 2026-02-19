#include <stdarg.h>
#include <stdio.h>
#include <time.h>

void my_error_message(const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  time_t current = time(NULL);
  struct tm *p = localtime(&current);
  fprintf(stderr, "%04i/%02i/%02i %02i:%02i:%02i ", 1900 + p->tm_year, p->tm_mon + 1, p->tm_mday,
          p->tm_hour, p->tm_min, p->tm_sec);
  vfprintf(stderr, format, ap);
  va_end(ap);
}
int main(void) { my_error_message("error %i was occurred at %s.\n", 32, "main"); }
