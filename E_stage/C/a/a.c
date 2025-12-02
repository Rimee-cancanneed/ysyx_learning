// a.c
#include <stdio.h>
void f(int x, int y) {
  printf("x = %d, y = %d\n", x, y);
}
int main() {
  int i = 1;
  int x = i ++, y = i ++;
  f(x, y);
  printf("int length: %zu bytes\n", sizeof(int));
  return 0;
}