#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

/*
 * File contains serve settings constants.
 */

constexpr uint16_t MESSAGE_INTERVAL{30};

constexpr uint32_t EVENT_META_DATA_LENGTH{sizeof(uint32_t) + sizeof(uint8_t)};

constexpr char LEFT_KEY_UP[]{"LEFT_KEY_UP\n"};
constexpr char LEFT_KEY_DOWN[]{"LEFT_KEY_DOWN\n"};
constexpr char RIGHT_KEY_UP[]{"RIGHT_KEY_UP\n"};
constexpr char RIGHT_KEY_DOWN[]{"RIGHT_KEY_DOWN\n"};

enum class Keys : uint8_t {
    NONE = 0,
    RIGHT = 1,
    LEFT = 2,
};

constexpr char DEFAULT_PLAYER_NAME[]{""};
constexpr char DEFAULT_GUI_HOST[]{"localhost"};
constexpr char DEFAULT_SERVER_PORT[]{"2021"};
constexpr char DEFAULT_GUI_PORT[]{"20210"};

#endif //CONSTANTS_H
