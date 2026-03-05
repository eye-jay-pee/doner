#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "watcher.h"
#include "countof.h"
static bool create_fifo(    struct watcher *self, const char *path);
static bool open_fifo(      struct watcher *self);
static bool create_epoll(   struct watcher *self);
static bool start_epoll(    struct watcher *self);
static bool setup_buffer(   struct watcher *self, size_t n);
static bool setup_response( struct watcher *self, void (*resp)(const char*));
struct watcher new_watcher( const char *path, void (*resp)(const char*)) {
  struct watcher self = {
      .all_ok = true,
  };
  self.all_ok &= create_fifo(&self, path);
  self.all_ok &= open_fifo(&self);
  self.all_ok &= create_epoll(&self);
  self.all_ok &= start_epoll(&self);
  self.all_ok &= setup_buffer(&self, DEFAULT_BUFFER_SIZE);
  self.all_ok &= setup_response(&self, resp);
  return self;
}
struct watcher with_buf_size(struct watcher self, size_t size) {
  self.all_ok &= setup_buffer(&self, size);
  return self;
}
static bool create_fifo(struct watcher *self, const char *path) {
  unlink(path);
  if (mkfifo(path, 0666) < 0) {
    perror("mkfifo");
    return false;
  } else {
    self->target_path = path;
    return true;
  }
}
static bool open_fifo(struct watcher *self) {
  int fd = open(self->target_path, O_RDONLY);
  if (fd < 0) {
    perror("open fifo");
    return false;
  } else {
    self->target_fd = fd;
    return true;
  }
}
static bool create_epoll(struct watcher *self) {
  int fd = epoll_create1(0);
  if (fd < 0) {
    perror("epoll_create1");
    return false;
  } else {
    self->epoll_fd = fd;
    return true;
  }
}
static bool start_epoll(struct watcher *self) {
  struct epoll_event config = {
      .events = EPOLLIN,
      .data.fd = self->target_fd,
  };
  if (epoll_ctl(self->epoll_fd, EPOLL_CTL_ADD, self->target_fd, &config) != 0) {
    perror("epoll_ctl");
    return false;
  } else
    return true;
}
static bool setup_buffer(struct watcher *self, size_t n) {
  char *buf = malloc(n);
  if (buf == NULL) {
    perror("malloc");
    return false;
  } else {
    free(self->buf);
    self->buf = buf;
    self->buf_n = n;
    return true;
  }
}
static bool setup_response( struct watcher *self, void (*resp)(const char*)) {  
    if(resp == NULL) {
        perror("fn pointer");
        return false;
    } else {
        self->response_fp = resp;
        return true;
    }
}
