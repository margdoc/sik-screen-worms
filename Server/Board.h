#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <vector>

#include "geometry.h"

/*
 * Class for handling game board data.
 */
class Board {
public:
    Board(uint32_t const width, uint32_t const height)
            : width{width}, height{height} {
        reset();
    }

    /*
     * Method checks if pixel is valid.
     */
    [[nodiscard]] bool isInside(const Pixel &pixel) const {
        return pixel.x < width &&
               pixel.y < height;
    }

    /*
     * Method returns board width.
     */
    [[nodiscard]] uint32_t getWidth() const {
        return width;
    }

    /*
     * Method returns board height.
     */
    [[nodiscard]] uint32_t getHeight() const {
        return height;
    }

    /*
     * Method handles eating pixel by player and returns true if it was eaten correctly.
     */
    bool eat(Pixel pixel);

    /*
     * Method handles creating new game.
     */
    void reset();

private:
    uint32_t const width, height;
    std::vector<std::vector<bool>> eaten;
};

#endif //BOARD_H
