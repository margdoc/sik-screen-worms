#ifndef EPOLL_H
#define EPOLL_H

#include <unistd.h>
#include <sys/epoll.h>
#include <map>
#include <cassert>

#include "err.h"
#include "FileDescriptor.h"

/*
 * Class for handling epoll operations. N is events queue size.
 */
template<uint8_t N>
class Epoll {
public:
    Epoll()
            : fd{epoll_create(1)}, events{} {
        if (fd == -1) {
            syserr("epoll_create1");
        }
    }

    ~Epoll() noexcept {
        if (close(fd)) {
            syserr("close epoll");
        }
    }

    /*
     * Method adds descriptor for listening.
     */
    void add(int descriptor, uint8_t priority = 1);

    /*
     * Method adds descriptor for listening.
     */
    void add(FileDescriptor const &fd, uint8_t priority = 1);

    /*
     * Method waits and returns next event.
     */
    struct epoll_event &wait();

private:
    int const fd;

    struct epoll_event events[N];
    std::map<int, uint8_t> priorities;
};


template<uint8_t N>
inline void Epoll<N>::add(int const descriptor, uint8_t const priority) {
    struct epoll_event event{};

    event.events = EPOLLIN;
    event.data.fd = descriptor;

    priorities.insert({descriptor, priority});

    if (epoll_ctl(fd, EPOLL_CTL_ADD, descriptor, &event) != 0) {
        syserr("epoll_ctl");
    }
}


template<uint8_t N>
void Epoll<N>::add(FileDescriptor const &fileDescriptor, uint8_t priority) {
    add(fileDescriptor.getDescriptor(), priority);
}


template<uint8_t N>
inline struct epoll_event &Epoll<N>::wait() {
    memset(events, 0, sizeof(events));

    int eventsCount;

    if ((eventsCount = epoll_wait(fd, events, N, -1)) < 0) {
        syserr("epoll_wait");
    }

    uint8_t maxPriority{0};

    // Find highest priority.
    for (auto const &event : events) {
        maxPriority = std::max(
                maxPriority,
                priorities[event.data.fd]
        );
    }

    // Take event from descriptor with highest priority.
    for (auto &event : events) {
        if (priorities[event.data.fd] == maxPriority) {
            return event;
        }
    }

    assert(false && "Valid event not found");
}


#endif //EPOLL_H
