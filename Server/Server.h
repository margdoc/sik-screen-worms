#ifndef SERVER_H
#define SERVER_H

#include <sys/epoll.h>
#include <unistd.h>
#include <functional>
#include <utility>
#include <set>
#include <iostream>

#include "../Common/err.h"
#include "../Common/Epoll.h"
#include "../Common/Timer.h"
#include "../Common/Socket.h"
#include "../Common/logger.h"

#include "Game.h"
#include "Client.h"
#include "constants.h"

/*
 * Class for handling server tasks i.e. reading and writing to sockets and
 * managing connected clients.
 */
class Server {
public:
    Server(uint32_t const port, uint32_t const roundsPerSecond, GameSettings const &settings)
            : socket{Protocol::UDP}, game{settings, *this}, roundsPerSecond{roundsPerSecond},
              round{0}, roundsToDisconnect(TIME_TO_DISCONNECT * roundsPerSecond) {
        logImportant("Server created on port ", port, ".");

        socket.bind(port);

        epoll.add(socket, 1);
        epoll.add(timer, 2);
    }

    /*
     * Method runs main server loop.
     */
    [[noreturn]] [[noreturn]] void run();

    /*
     * Method sends content of buffer to player.
     */
    void sendToPlayer(std::shared_ptr<Player> const &player, OutputBuffer const &buffer) const;

private:
    /*
     * Method runs game round (if it's running) and checks if any client goes silent.
     */
    void newRound();

    /*
     * Method reads message from socket, parses it and executes it.
     */
    void receiveMessage();

    /*
     * Method executes player message.
     */
    void executeMessage(sockaddr_in6 const &clientAddress, uint32_t sessionId, uint8_t turnDirection,
                        uint32_t nextExpectedEventNo, std::string const &playerName);

    /*
     * Method creates new player for given client.
     */
    void createPlayer(std::shared_ptr<Client> const &client, std::string const &playerName);

    Socket const socket; // Main socket for listening for messages.
    Game game;
    Timer const timer;  // Rounds timer.
    uint32_t const roundsPerSecond;
    Epoll<3> epoll; // Epoll for waiting for message from socket or timer.
    uint64_t round; // Current round counter.
    uint16_t const roundsToDisconnect; // Number of rounds after which client is disconnected.
    std::set<std::shared_ptr<Client>, Client::comparator> clients; // Set of all connected clients.
    InputBuffer socketBuffer; // Buffer for incoming messages.
};

#endif //SERVER_H
