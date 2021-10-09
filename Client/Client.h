#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string>
#include <utility>
#include <sys/epoll.h>
#include <vector>
#include <chrono>

#include "../Common/Timer.h"
#include "../Common/Epoll.h"
#include "../Common/Socket.h"

#include "Player.h"
#include "MessageParser.h"
#include "Game.h"

/*
 * Class for handling sending and receiving messages.
 */
class Client {
public:
    Client(std::string name, std::string const &serverHost, std::string const &serverPort,
           std::string const &guiHost, std::string const &guiPort)
            : server{Socket::getFromAddress(serverHost, serverPort, Protocol::UDP)},
              gui{Socket::getFromAddress(guiHost, guiPort, Protocol::TCP)},
              sessionId{static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(
                      std::chrono::system_clock::now().time_since_epoch()
              ).count())}, parser{*this}, player{std::move(name)} {
        gui.disableNagleAlgorithm();

        epoll.add(timer, 2);
        epoll.add(server, 1);
        epoll.add(gui, 1);
    }

    Game &getGame() {
        return game;
    }

    /*
     * Method runs main clien loop.
     */
    [[noreturn]] void run();

    /*
     * Method handles new game event.
     */
    void newGame(uint32_t maxx, uint32_t maxy, std::vector<std::string> const &players);

    /*
     * Method handles pixel event.
     */
    void pixel(uint32_t x, uint32_t y, uint8_t playerIndex);

    /*
     * Method handles player eliminated event.
     */
    void playerEliminated(uint8_t playerIndex);

    /*
     * Method handles game over event.
     */
    void gameOver();

private:
    /*
     * Method sends buffer content to server.
     */
    void sendMessage() const;

    /*
     * Method receives and parses message from server.
     */
    void receiveMessage();

    /*
     * Method receives and parses message from gui.
     */
    void receiveAction();

    /*
     * Method sends message to gui.
     */
    void sendMessageToGui(std::string const &message) const;

    Socket server, gui;
    Timer const timer;
    Game game;
    uint64_t const sessionId;
    MessageParser parser;
    Epoll<4> epoll;
    Player player;
};

#endif //CLIENT_H
