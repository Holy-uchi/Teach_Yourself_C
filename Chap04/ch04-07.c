#include <stdio.h>
#include <stdbool.h>

// bool even(int n)
// {
//   return (n % 2 == 0) ? true : false;
// }
bool even(int n)
{
  if (n % 2 == 0) {
    return true;
  } else {
    return false;
  }
}

int main(void)
{
  bool a = even(112);
  printf("%i\n", a);
}