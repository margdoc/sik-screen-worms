#include "Player.h"


void Player::newGame(Position const startingPosition, uint16_t const startingDirection) {
    position = startingPosition;
    previousPixel = roundPosition(startingPosition);
    direction = startingDirection;
    alive = true;
    ready = false;
}


void Player::gameOver() {
    ready = false;
}


void Player::setTurnDirection(Direction const turnedDirection) {
    turnDirection = turnedDirection;
    ready = ready || (turnDirection != Direction::NONE);
}


void Player::move(uint16_t turningSpeed) {
    previousPixel = getPixel();

    if (turnDirection == Direction::RIGHT) {
        direction += turningSpeed;
        direction %= 360;
    } else if (turnDirection == Direction::LEFT) {
        if (direction < turningSpeed) {
            direction += 360 - turningSpeed;
        } else {
            direction -= turningSpeed;
        }
    }

    position += rotationVector(direction);
}


void Player::disconnect() {
    client = nullptr;
}