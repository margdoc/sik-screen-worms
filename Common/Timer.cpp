#include "Timer.h"


void Timer::start(uint32_t miliseconds) const {
    struct itimerspec specification{};

    specification.it_interval = specification.it_value = {
            .tv_sec =  miliseconds / 1000,
            .tv_nsec = (miliseconds % 1000) * 1000000,
    };

    if (timerfd_settime(fd, 0, &specification, nullptr) < 0) {
        syserr("timerfd_settime");
    }
}


void Timer::stop() const {
    struct itimerspec specification{};

    specification.it_interval = specification.it_value = {
            .tv_sec = 0,
            .tv_nsec = 0,
    };

    if (timerfd_settime(fd, 0, &specification, nullptr) < 0) {
        syserr("timerfd_settime");
    }
}


void Timer::unmark() const {
    uint64_t response;

    if (::read(fd, &response, sizeof(response)) < 0) {
        syserr("read timer");
    }
}