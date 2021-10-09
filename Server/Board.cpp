#include "Board.h"


bool Board::eat(Pixel const pixel) {
    if (eaten[pixel.x][pixel.y]) {
        return false;
    } else {
        return eaten[pixel.x][pixel.y] = true;
    }
}


void Board::reset() {
    eaten = std::vector{width, std::vector<bool>(height, false)};
}
