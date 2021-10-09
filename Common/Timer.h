#ifndef TIMER_H
#define TIMER_H

#include <sys/timerfd.h>
#include <cinttypes>

#include "err.h"
#include "FileDescriptor.h"

/*
 * Class handles timerfd operations.
 */
class Timer : public FileDescriptor {
public:
    Timer()
            : FileDescriptor{timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK)} {}

    ~Timer() override {
        stop();
    }

    /*
     * Method starts timerfd counter with given interval length.
     */
    void start(uint32_t miliseconds) const;

    /*
     * Method stops timerfd counter.
     */
    void stop() const;

    /*
     * Method marks that timerfd output was read.
     */
    void unmark() const;
};


#endif //TIMER_H
