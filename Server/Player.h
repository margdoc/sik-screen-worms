#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <utility>
#include <memory>

#include "../Common/constants.h"

#include "geometry.h"

class Client;

/*
 * Class for handling players.
 */
class Player {
public:
    explicit Player(std::string playerName, std::shared_ptr<Client> client) :
            name{std::move(playerName)}, observer{name.empty()}, turnDirection{0}, position{0, 0},
            previousPixel{0, 0}, direction{0}, alive{true}, ready{false}, client{std::move(client)} {}

    /*
     * Method sets player state for new game.
     */
    void newGame(Position startingPosition, uint16_t startingDirection);

    /*
     * Method sets player state for game over.
     */
    void gameOver();

    /*
     * Methods sets player turn direction.
     */
    void setTurnDirection(Direction turnedDirection);

    /*
     * Method calculate player position in new turn.
     */
    void move(uint16_t turningSpeed);

    /*
     * Method handles player disconnection.
     */
    void disconnect();

    /*
     * Method checks if player changed pixel in current round.
     */
    [[nodiscard]] bool changedPixel() const {
        return getPreviousPixel() != getPixel();
    }

    /*
     * List of player attributes setters and getters.
     */
    void setAlive(bool const isAlive) {
        alive = isAlive;
    }

    [[nodiscard]] bool isAlive() const {
        return alive;
    }

    [[nodiscard]] bool isReady() const {
        return ready;
    }

    [[nodiscard]] bool isObserver() const {
        return observer;
    }

    [[nodiscard]] std::string getName() const {
        return name;
    }

    [[nodiscard]] Pixel getPixel() const {
        return roundPosition(position);
    }

    [[nodiscard]] Pixel getPreviousPixel() const {
        return previousPixel;
    }

    [[nodiscard]] std::shared_ptr<Client> getClient() const {
        return client;
    };

    /*
     * Comparator for handling players in alphabetical order.
     */
    struct comparator {
        using is_transparent = void;

        bool operator()(std::shared_ptr<Player> const &lhs, std::shared_ptr<Player> const &rhs) const {
            return
                    (lhs == nullptr ? "" : lhs->getName())
                    <
                    (rhs == nullptr ? "" : rhs->getName());
        }

        bool operator()(std::shared_ptr<Player> const &lhs, std::string const &rhs) const {
            return lhs->getName() < rhs;
        }

        bool operator()(std::string const &lhs, std::shared_ptr<Player> const &rhs) const {
            return lhs < rhs->getName();
        }
    };

private:
    std::string const name;
    bool const observer; // Boolean telltale if player is game observer.
    Direction turnDirection;
    Position position;
    Pixel previousPixel; // Pixel on which player was in previous turn.
    uint16_t direction;
    bool alive;
    bool ready;
    std::shared_ptr<Client> client;
};

#endif //PLAYER_H
