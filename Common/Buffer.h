#ifndef BUFFER_H
#define BUFFER_H

#include <cstdint>
#include <functional>
#include <utility>
#include <string>
#include <cstring>
#include <endian.h>
#include <sys/socket.h>
#include <cassert>


class FileDescriptor;
class Socket;

/*
 * Function changes endianness from host to big endian.
 */
template<typename T>
static T hostToBigEndian(T value);

/*
 * Function changes endianness from big endian to host.
 */
template<typename T>
static T bigEndianToHost(T value);

/*
 * Class for handling bytes sequence.
 */
class Buffer {
public:
    Buffer()
            : position{0}, buffer{} {}

    [[nodiscard]] void const *getBuffer() const {
        return buffer;
    }

    [[nodiscard]] void *getBuffer() {
        return buffer;
    }

    /*
     * Method returns next empty byte in buffer.
     */
    [[nodiscard]] void const *getNextEmpty() const {
        return buffer + position;
    }

    [[nodiscard]] static uint32_t getMaxSize() {
        return BUFFER_SIZE;
    }

    /*
     * Method handles clearing buffer.
     */
    void reset() {
        position = 0;
    }

protected:
    static constexpr uint32_t BUFFER_SIZE{550};

    uint32_t position;
    char buffer[BUFFER_SIZE];
};

/*
 * Class for storing messages from file descriptors.
 */
class InputBuffer : public Buffer {
public:
    InputBuffer()
            : Buffer{}, size{0} {}

    /*
     * Operator reads data from buffer.
     */
    template<typename T>
    InputBuffer &operator>>(T &t);

    [[nodiscard]] int64_t remaining() const {
        return size - position;
    }

    /*
     * Method reads data from buffer but doesn't store or return it.
     */
    void skip(uint32_t toSkip) {
        assert(position + toSkip <= size);
        position += toSkip;
    }

    /*
     * Method reads all bytes from buffer and stores it in string.
     */
    std::string readAll() {
        std::string const string{buffer + position, size - position};
        position = size;
        return string;
    }

    friend FileDescriptor;
    friend Socket;
private:
    uint32_t size;
};

/*
 * Class for storing messages to send to file descriptors.
 */
class OutputBuffer : public Buffer {
public:
    OutputBuffer()
        : Buffer{} {}

    /*
     * Method writes data to buffer.
     */
    template<typename T>
    OutputBuffer &operator<<(T const &t);

    [[nodiscard]] uint32_t getSize() const {
        return position;
    }

    [[nodiscard]] uint32_t getEmptySpace() const {
        return BUFFER_SIZE - position;
    }
};


template<>
inline uint8_t hostToBigEndian<uint8_t>(uint8_t value) {
    return value;
}

template<>
inline uint16_t hostToBigEndian<uint16_t>(uint16_t value) {
    return htobe16(value);
}

template<>
inline uint32_t hostToBigEndian<uint32_t>(uint32_t value) {
    return htobe32(value);
}

template<>
inline uint64_t hostToBigEndian<uint64_t>(uint64_t value) {
    return htobe64(value);
}

template<>
inline uint8_t bigEndianToHost<uint8_t>(uint8_t value) {
    return value;
}

template<>
inline uint16_t bigEndianToHost<uint16_t>(uint16_t value) {
    return be16toh(value);
}

template<>
inline uint32_t bigEndianToHost<uint32_t>(uint32_t value) {
    return be32toh(value);
}

template<>
inline uint64_t bigEndianToHost<uint64_t>(uint64_t value) {
    return be64toh(value);
}

template<>
inline InputBuffer &InputBuffer::operator>> <std::string>(std::string &t) {
    t.clear();
    while (position < size && buffer[position] != '\0') {
        t += buffer[position++];
    }
    assert(position <= size);
    ++position;
    return *this;
}

template<>
inline InputBuffer &InputBuffer::operator>> <char>(char &t) {
    assert(position < size);
    t = buffer[position++];
    return *this;
}

template<typename T>
inline InputBuffer &InputBuffer::operator>>(T &t) {
    static_assert(std::is_trivial_v<T>);
    assert(position + sizeof(T) <= size);
    memcpy(&t, buffer + position, sizeof(T));
    t = bigEndianToHost<T>(t);
    position += sizeof(T);
    return *this;
}

template<>
inline OutputBuffer &OutputBuffer::operator<< <std::string>(std::string const &t) {
    assert(position + t.size() <= getMaxSize());
    memcpy(buffer + position, t.c_str(), t.size());
    position += t.size();
    return *this;
}

template<>
inline OutputBuffer &OutputBuffer::operator<< <char>(char const &t) {
    assert(position < getMaxSize());
    buffer[position++] = t;
    return *this;
}

template<typename T>
inline OutputBuffer &OutputBuffer::operator<<(T const &t) {
    static_assert(std::is_trivial_v<T>);
    assert(position + sizeof(T) <= getMaxSize());
    T const _t = hostToBigEndian<T>(t);
    memcpy(buffer + position, &_t, sizeof(T));
    position += sizeof(T);
    return *this;
}

#endif //BUFFER_H
