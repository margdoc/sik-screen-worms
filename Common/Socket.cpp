#include "Socket.h"

#include <netdb.h>
#include <uv.h>
#include <cassert>


static void checkErrno(char const *message) {
    if (errno != EAGAIN && errno != EWOULDBLOCK) {
        syserr(message);
    }
}


void Socket::disableNagleAlgorithm() {
    int const flag{1};

    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *) &flag, sizeof(int)) < 0) {
        syserr("setsockopt");
    }
}


Socket Socket::getFromAddress(std::string const &host, std::string const &port,
                              Protocol const protocol) {
    struct addrinfo addr_hints{};
    struct addrinfo *addr_result, *rp;

    int sock{};

    memset(&addr_hints, 0, sizeof(struct addrinfo));
    addr_hints.ai_family = AF_UNSPEC;
    addr_hints.ai_socktype = getSocketType(protocol);
    addr_hints.ai_protocol = getProtocol(protocol);

    if (getaddrinfo(host.c_str(), port.c_str(), &addr_hints, &addr_result) != 0) {
        syserr("getaddrinfo");
    }

    for (rp = addr_result; rp != nullptr; rp = rp->ai_next) {
        if ((sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) < 0) {
            continue;
        }

        if (::connect(sock, rp->ai_addr, rp->ai_addrlen) == 0) {
            break;
        }

        close(sock);
    }

    freeaddrinfo(addr_result);

    if (rp == nullptr) {
        syserr("connect");
    }

    return Socket{sock};
}


void Socket::bind(uint32_t port) const {
    sockaddr_in6 address{};

    address.sin6_family = AF_INET6;
    address.sin6_addr = in6addr_any;
    address.sin6_port = htons(port);

    if (::bind(fd, reinterpret_cast<struct sockaddr *>(&address),
            static_cast<socklen_t>(sizeof(address))) < 0) {
        syserr("bind");
    }
}


void Socket::send(OutputBuffer const &buffer) const {
    if (::send(fd, buffer.getBuffer(), buffer.getSize(), 0) != buffer.getSize()) {
        checkErrno("send");
    }
}


void Socket::send(OutputBuffer const &buffer, struct sockaddr *address, socklen_t socklen) const {
    if (::sendto(fd, buffer.getBuffer(), buffer.getSize(), 0,
                 address, socklen) != buffer.getSize()) {
        checkErrno("sendto");
    }
}

void Socket::receive(InputBuffer &buffer, struct sockaddr *address, socklen_t *socklen) const {
    buffer.reset();
    ssize_t length;

    if ((length = ::recvfrom(fd, buffer.getBuffer(), InputBuffer::getMaxSize(), 0,
                             address, socklen)) < 0) {
        checkErrno("recvfrom");
    }

    buffer.size = length;
}


int Socket::getSocketType(Protocol const protocol) {
    switch (protocol) {
        case Protocol::TCP:
            return SOCK_STREAM;
        case Protocol::UDP:
            return SOCK_DGRAM;
        default:
            assert(false && "Invalid protocol");
    }
}


int Socket::getProtocol(Protocol const protocol) {
    switch (protocol) {
        case Protocol::TCP:
            return IPPROTO_TCP;
        case Protocol::UDP:
            return IPPROTO_UDP;
        default:
            assert(false && "Invalid protocol");
    }
}
