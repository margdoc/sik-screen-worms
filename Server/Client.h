#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <cstdint>
#include <memory>
#include <netdb.h>
#include <optional>

#include "../Common/Socket.h"
#include "../Common/Buffer.h"

#include "Player.h"

/*
 * Class for handling connected clients.
 */
class Client {
public:
    explicit Client(sockaddr_in6 const &address) :
            address{address}, sessionId{}, lastMessage{0}, player{std::nullopt} {}

    sockaddr_in6 const address;
    uint64_t sessionId;
    uint64_t lastMessage;

    /*
     * Method checks if client has connected player.
     */
    [[nodiscard]] bool hasPlayer() const {
        return player.has_value();
    }

    /*
     * Method returns connected player.
     */
    [[nodiscard]] std::shared_ptr<Player> getPlayer() const {
        return player.value().lock();
    }

    /*
     * Method sets connected player.
     */
    void setPlayer(std::shared_ptr<Player> const &playerPtr) {
        player = playerPtr;
    }

    /*
     * Methods sends buffer content to client.
     */
    void send(Socket const &socket, OutputBuffer const &buffer) const {
        socket.send(buffer, (struct sockaddr *) &address, (socklen_t) sizeof(address));
    }

    /*
     * Method handles disconnection of client.
     */
    void disconnect();

    /*
     * Comparator for linear order for players by their clients.
     */
    struct comparatorForPlayers {
        using is_transparent = void;

        bool operator()(sockaddr_in6 const &lhs, sockaddr_in6 const &rhs) const {
            return 0 < memcmp(&lhs, &rhs, sizeof(sockaddr_in6));
        }

        bool operator()(std::shared_ptr<Client> const &lhs, std::shared_ptr<Client> const &rhs) const {
            return operator()(lhs->address, rhs->address);
        }

        bool operator()(std::shared_ptr<Player> const &lhs, std::shared_ptr<Player> const &rhs) const {
            return operator()(lhs->getClient(), rhs->getClient());
        }

        bool operator()(std::shared_ptr<Client> const &lhs, std::shared_ptr<Player> const &rhs) const {
            return operator()(lhs, rhs->getClient());
        }

        bool operator()(std::shared_ptr<Player> const &lhs, std::shared_ptr<Client> const &rhs) const {
            return operator()(lhs->getClient(), rhs);
        }
    };

    /*
     * Comparator for linear order for clients.
     */
    struct comparator {
        using is_transparent = void;

        bool operator()(sockaddr_in6 const &lhs, sockaddr_in6 const &rhs) const {
            return 0 < memcmp(&lhs, &rhs, sizeof(sockaddr_in6));
        }

        bool operator()(std::shared_ptr<Client> const &lhs, std::shared_ptr<Client> const &rhs) const {
            return operator()(lhs->address, rhs->address);
        }

        bool operator()(std::shared_ptr<Player> const &lhs, std::shared_ptr<Player> const &rhs) const {
            return operator()(lhs->getClient(), rhs->getClient());
        }

        bool operator()(std::shared_ptr<Client> const &lhs, std::shared_ptr<Player> const &rhs) const {
            return operator()(lhs, rhs->getClient());
        }

        bool operator()(std::shared_ptr<Player> const &lhs, std::shared_ptr<Client> const &rhs) const {
            return operator()(lhs->getClient(), rhs);
        }

        bool operator()(std::shared_ptr<Client> const &lhs, sockaddr_in6 const &rhs) const {
            return operator()(lhs->address, rhs);
        }

        bool operator()(sockaddr_in6 const &lhs, std::shared_ptr<Client> const &rhs) const {
            return operator()(lhs, rhs->address);
        }
    };

private:
    std::optional<std::weak_ptr<Player>> player;
};


#endif //CLIENT_H
