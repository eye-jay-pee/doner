#include <stdio.h>
#include "watcher.h"

void response(const char * input) {
    printf("%s", input);
    fflush(stdout);
}

int main(void) {
  struct watcher w = new_watcher("/tmp/demo_fifo", response);
  while (w.all_ok)
    watch(&w);
  return -1;
}
