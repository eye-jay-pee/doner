#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "countof.h"
#include "inbox.h"

/// checks if fifo exists at path.
static bool fifo_does_exist(struct fifo_watcher *self) {
  struct stat st;
  return (stat(self->fifo_path, &st) == 0) && S_ISFIFO(st.st_mode);
}
/// obtains a file descriptor for fifo at path.
static bool fifo_setup(struct fifo_watcher *self) {
  if (fifo_does_exist(self) || (mkfifo(self->fifo_path, 0666) == 0))
    return (self->fifo_fd = open(self->fifo_path, O_RDONLY | O_NONBLOCK)) >= 0;
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
static bool fifo_read_line(struct fifo_watcher *self) {
    printf("test");
 (void)self;
  //char buf[1024];
  //ssize_t n;


  return true;
  // while((n = read(self->fifo_fd, buf, sizeof(buf))) > 0) {
  //   for (ssize_t i = 0; i < n; i++) {
  //     putchar(buf[i]);
  //     if (buf[i] == '\n') {
  //       fflush(stdout);
  //       return true;
  //     }
  //   }
  // }
  // if (n == 0) {
  //     fflush(stdout);
  //     close(self->fifo_fd);
  //     return fifo_setup(self);
  // }
  // return false;
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
int watch_fifo_line(struct fifo_watcher *self) {
  while(self->all_ok) {
    self->all_ok &= (epoll_wait(self->epoll_fd, &self->trigger, 1, -1) == 1);
    self->all_ok &= fifo_read_line(self);
  }
  return -1;
}
