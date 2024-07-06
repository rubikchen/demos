#include <stdio.h>
void __attribute__((visibility("default"))) good() {
  printf("xxx %s\n", __func__);
}
void __attribute__((visibility("hidden"))) show() { good(); }
