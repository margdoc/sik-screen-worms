#ifndef FILEDESCRIPTOR_H
#define FILEDESCRIPTOR_H

#include <unistd.h>

#include "err.h"
#include "Buffer.h"

/*
 * Abstract class for file descriptor handling.
 */
class FileDescriptor {
public:
    explicit FileDescriptor(int const fd)
            : fd{fd}, file{nullptr} {
        if (fd < 0) {
            syserr("fd");
        }
    }

    virtual ~FileDescriptor() {
        if (close(fd) != 0) {
            syserr("close");
        }
    }

    [[nodiscard]] int getDescriptor() const {
        return fd;
    }

    /*
     * Method writes buffer content to descriptor.
     */
    void write(std::string const &buffer) const;

    /*
     * Method reads from descriptor to buffer.
     */
    [[nodiscard]] InputBuffer read() const;

    /*
     * Method reads entire line from descriptor.
     */
    std::string getLine();

protected:
    int const fd;

private:
    FILE *file;
};

#endif //FILEDESCRIPTOR_H
