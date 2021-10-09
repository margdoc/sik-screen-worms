#ifndef GAME_H
#define GAME_H

#include <utility>
#include <vector>
#include <memory>
#include <set>
#include <functional>

#include "../Common/Timer.h"

#include "RandomGenerator.h"
#include "Event.h"
#include "Player.h"
#include "Client.h"
#include "Board.h"

class Server;

/*
 * Class is container for game setup settings.
 */
class GameSettings {
public:
    GameSettings(uint32_t const seed, uint32_t const turningSpeed,
                 uint32_t const width, uint32_t const height)
            : seed{seed}, turningSpeed{turningSpeed}, width{width}, height{height} {}


    uint32_t const seed;
    uint32_t const turningSpeed;
    uint32_t const width, height;
};

/*
 * Class for handling game rounds and events.
 */
class Game {
public:
    Game(GameSettings const &settings, Server &server)
            : server{server}, board{settings.width, settings.height},
              generator{settings.seed}, turningSpeed{settings.turningSpeed},
              isRunning{false}, newGame{false}, gameOver{false}, id{}, alivePlayers{} {}

    /*
     * Method creates player.
     */
    std::shared_ptr<Player> newPlayer(std::string const &playerName, std::shared_ptr<Client> const &client);

    /*
     * Method handles player disconnection.
     */
    void disconnectPlayer(std::shared_ptr<Client> const &client);

    /*
     * Method returns player from set of players with name.
     */
    [[nodiscard]] std::shared_ptr<Player> getPlayer(std::string const &playerName) const;

    /*
     * Method handles message from player.
     */
    void playerMessage(std::shared_ptr<Client> const &client, uint8_t turnDirection,
                       uint32_t nextExpectedEventNo);

    /*
     * Method runs new turn.
     */
    void nextTurn();

private:
    /*
     * Methods sends content of events buffer to all players.
     */
    void flushBuffer();

    /*
     * Method handles events generation.
     */
    template<typename T, typename = std::enable_if_t<std::is_base_of_v<Event, T>>, typename ...Args>
    void generateEvent(Args &&...args) {
        std::shared_ptr<Event> const event = std::make_shared<T>(events.size(), args...);

        if (eventsBuffer.getSize() == 0) {
            eventsBuffer << id;
        }

        if (eventsBuffer.getEmptySpace() < event->getAllDataSize()) {
            flushBuffer();
        }

        event->write(eventsBuffer);
        events.push_back(event);
    }

    /*
     * Method handles player elimination event.
     */
    void eliminatePlayer(uint8_t playerId, std::shared_ptr<Player> const &player);

    /*
     * Method starts new game.
     */
    void start();

    /*
     * Method handles end of current game.
     */
    void end();

    OutputBuffer eventsBuffer; // Buffer of events prepared to send.

    std::set<std::shared_ptr<Player>, Client::comparatorForPlayers> observers;
    std::set<std::shared_ptr<Player>, Player::comparator> readyPlayers;
    std::set<std::shared_ptr<Player>, Player::comparator> allPlayers;
    std::vector<std::shared_ptr<Player>> inGamePlayers;

    Server &server;
    Board board;
    RandomGenerator generator;
    std::vector<std::shared_ptr<Event>> events; // Queue of all events in current game.

    uint32_t const turningSpeed;
    bool isRunning; // Boolean telltale if game is running.
    bool newGame; // Boolean telltale if new game can be started.
    bool gameOver; // Boolean telltale if game is ended.
    uint32_t id;
    uint32_t alivePlayers; // Counter of alive players.

};


#endif //GAME_H
