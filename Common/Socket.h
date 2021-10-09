#ifndef SOCKET_H
#define SOCKET_H

#include <unistd.h>

#include "err.h"
#include "Buffer.h"
#include "FileDescriptor.h"

/*
 * Enum of possible protocol types.
 */
enum class Protocol {
    UDP, TCP
};

/*
 * Class handles socket operations.
 */
class Socket : public FileDescriptor {
public:
    explicit Socket(Protocol const protocol)
            : Socket{socket(AF_INET6, getSocketType(protocol), 0)} {}

    /*
     * Class method returns connection socket from host address.
     */
    static Socket getFromAddress(std::string const &host, std::string const &port,
                                 Protocol protocol);

    /*
     * Method disables nagle algorithm.
     */
    void disableNagleAlgorithm();

    /*
     * Method binds socket on given port.
     */
    void bind(uint32_t port) const;

    /*
     * Method sends buffer content through network.
     */
    void send(OutputBuffer const &buffer) const;

    /*
     * Method sends buffer content through network to given address.
     */
    void send(OutputBuffer const &buffer, struct sockaddr *address, socklen_t socklen) const;

    /*
     * Method reads buffer from network and sets sender address.
     */
    void receive(InputBuffer &buffer, struct sockaddr *address, socklen_t *socklen) const;

private:
    explicit Socket(int const sock)
            : FileDescriptor{sock} {}

    /*
     * Method returns socket type from protocol.
     */
    static int getSocketType(Protocol protocol);

    /*
     * Method returns c protocol type from protocol.
     */
    static int getProtocol(Protocol protocol);
};

#endif //SOCKET_H
