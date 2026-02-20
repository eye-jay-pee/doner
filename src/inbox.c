#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>

#include "countof.h"
#include "inbox.h"

/// checks if fifo exists at path.
static bool fifo_does_exist(struct fifo_watcher *self) {
  struct stat st;
  return (stat(self->fifo_path, &st) == 0) && S_ISFIFO(st.st_mode);
}
/// obtains a file descriptor for fifo at path.
static bool fifo_setup(struct fifo_watcher *self) {
  const char *path = self->fifo_path;
  if (fifo_does_exist(self) || (mkfifo(path, 0666) == 0))
    return ((self->fifo_fd = open(path, O_RDONLY | O_NONBLOCK)) >= 0);
  else
    return false;
}
/// obtains file descriptor for epoll.
static bool epoll_setup(struct fifo_watcher *self) {
  if ((self->epoll_fd = epoll_create1(0)) < 0)
    return -1;
  else {
    self->trigger.events = EPOLLIN;
    self->trigger.data.fd = self->fifo_fd;
    return (epoll_ctl(self->epoll_fd, EPOLL_CTL_ADD, self->fifo_fd,
                      &self->trigger) >= 0);
  }
}
/// creates a new fifo watcher on the given path
struct fifo_watcher new_watcher(const char *fifo_path) {
  struct fifo_watcher self = { 
      .fifo_path = fifo_path, 
      .all_ok = true,
  };
  self.all_ok &= fifo_setup(&self);
  self.all_ok &= epoll_setup(&self);
  return self;
}
int watch_fifo(struct fifo_watcher *self) {
  while(self->all_ok) {
    printf("loop entered\t");
    self->all_ok &= (epoll_wait(self->epoll_fd, &self->trigger, 1, -1) == 1);
    printf("event detected\n");

  }
  return -1;
}
