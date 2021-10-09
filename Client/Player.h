#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <utility>

#include "constants.h"

/*
 * Class for handling player data.
 */
class Player {
public:
    explicit Player(std::string name)
            : name{std::move(name)}, pressedKey{Keys::NONE} {}

    /*
     * Method parses and executes action received from gui.
     */
    void guiAction(std::string const &action);

    [[nodiscard]] inline std::string getName() const {
        return name;
    }

    [[nodiscard]] inline uint8_t getTurnDirection() const {
        return static_cast<uint8_t>(pressedKey);
    }

    /*
     * Method handles end of game.
     */
    void gameOver();

private:
    std::string const name;
    Keys pressedKey;
};


#endif //PLAYER_H
