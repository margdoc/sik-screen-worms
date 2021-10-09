#include "FileDescriptor.h"


void FileDescriptor::write(std::string const &buffer) const {
    if (::write(fd, buffer.c_str(), buffer.size()) != static_cast<ssize_t>(buffer.size())) {
        syserr("write FileDescriptor");
    }
}


InputBuffer FileDescriptor::read() const {
    InputBuffer buffer;

    ssize_t length;

    if ((length = ::read(fd, buffer.getBuffer(), InputBuffer::getMaxSize())) < 0) {
        syserr("read FileDescriptor");
    }

    buffer.size = length;

    return buffer;
}


std::string FileDescriptor::getLine() {
    if (file == nullptr) {
        file = fdopen(fd, "r");
    }

    size_t size{0};
    ssize_t length;
    char *line;

    if ((length = ::getline(&line, &size, file)) == -1) {
        return "";
    }

    return std::string{line, static_cast<uint32_t>(length)};
}