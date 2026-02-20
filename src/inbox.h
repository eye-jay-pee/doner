#ifndef INBOX_H
#include <stdbool.h>
#include <sys/epoll.h>

struct fifo_watcher {
    const char * fifo_path;
    int fifo_fd;
    int epoll_fd;
    struct epoll_event trigger;
    void(*response)(void);
    bool all_ok;
};
struct fifo_watcher new_watcher(const char *fifo_path);
int watch_fifo(struct fifo_watcher *self);
#define INBOX_H
#endif
