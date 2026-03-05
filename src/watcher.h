#ifndef WATCHER_H
#include <stdbool.h>
#include <sys/epoll.h>
#define DEFAULT_BUFFER_SIZE 4096
struct watcher {
  const char *target_path;
  int target_fd;
  int epoll_fd;

  char * buf;
  size_t buf_n;

  struct epoll_event trigger;
  void (*response_fp)(const char*);
  bool all_ok;
};
struct watcher new_watcher(const char *path, void (*resp)(const char*));
struct watcher with_buf_size(struct watcher self, size_t size);
void watch(struct watcher *self);
#define WATCHER_H
#endif

