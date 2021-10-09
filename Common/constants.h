#ifndef COMMON_CONSTANTS_H
#define COMMON_CONSTANTS_H

#include <cstdint>

/*
 * Struct represents game and server settings.
 */
template<typename T>
struct settings {
    char const *name;
    char const flag;
    T const defaultValue;
    T const min;
    T const max;
};


enum class Direction : uint8_t {
    NONE = 0,
    RIGHT = 1,
    LEFT = 2
};


constexpr uint8_t MAX_EVENT_TYPE{3};

#endif //COMMON_CONSTANTS_H
