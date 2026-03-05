#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "watcher.h"
static bool wait_for_epoll(struct watcher *self);
static bool respond(struct watcher *self);
void watch(struct watcher *self) {
  self->all_ok &= wait_for_epoll(self);
  self->all_ok &= respond(self);
}
static bool wait_for_epoll(struct watcher *self) {
  const int event_capacity = 1;
  const int no_timeout = -1;
  int n =
      epoll_wait(self->epoll_fd, &self->trigger, event_capacity, no_timeout);

  if (n != event_capacity) {
    perror("epoll_wait");
    return false;
  } else
    return true;
}
static bool respond(struct watcher *self) {
  ssize_t r = read(self->target_fd, self->buf, self->buf_n - 1);
  if (r < 0) {
    perror("read error");
    return false;
  } else {
    self->buf[r] = '\0';
    self->response_fp((const char *) self->buf);
    return true;
  }
}

