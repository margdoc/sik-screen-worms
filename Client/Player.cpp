#include "Player.h"

#include "../Common/logger.h"


void Player::guiAction(std::string const &action) {
    if (action == LEFT_KEY_DOWN) {
        pressedKey = Keys::LEFT;
    } else if (action == LEFT_KEY_UP) {
        if (pressedKey == Keys::LEFT) {
            pressedKey = Keys::NONE;
        }
    } else if (action == RIGHT_KEY_DOWN) {
        pressedKey = Keys::RIGHT;
    } else if (action == RIGHT_KEY_UP) {
        if (pressedKey == Keys::RIGHT) {
            pressedKey = Keys::NONE;
        }
    } else {
        logError("Incorrect gui action\n|", action, "|");
    }
}


void Player::gameOver() {
    pressedKey = Keys::NONE;
}
