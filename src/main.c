#include "inbox.h"

int main(void) {
  struct fifo_watcher w = new_watcher("./gcode_queue");
  return watch_fifo(&w);
}
