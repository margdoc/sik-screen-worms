#include "geometry.h"

#include <cmath>

/*
 * Math constants.
 */
constexpr double PI{3.14159265};
constexpr double TO_RADIANS{PI / 180.};


Position rotationVector(uint16_t const angle) {
    return Position{
            cos(angle * TO_RADIANS),
            sin(angle * TO_RADIANS)
    };
}


Pixel roundPosition(Position const pos) {
    return Pixel{
            static_cast<uint32_t>(floor(pos.x)),
            static_cast<uint32_t>(floor(pos.y))
    };
}