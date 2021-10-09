#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "../Common/constants.h"

/*
 * File contains serve settings constants.
 */

constexpr uint32_t MAX_PLAYER_NAME_LENGTH{20};
constexpr uint8_t MIN_NAME_CHAR{33};
constexpr uint8_t MAX_NAME_CHAR{126};


constexpr settings<uint16_t> PORT_SETTINGS{
        .name = "port",
        .flag = 'p',
        .defaultValue = 2021,
        .min = 2000,
        .max = 65535
};

constexpr settings<uint32_t> TURNING_SPEED_SETTINGS{
        .name = "turning speed",
        .flag = 't',
        .defaultValue = 6,
        .min = 1,
        .max = 90
};

constexpr settings<uint32_t> ROUNDS_PER_SECOND_SETTINGS{
        .name = "rounds per second",
        .flag = 'v',
        .defaultValue = 50,
        .min = 1,
        .max = 250
};

constexpr settings<uint32_t> WIDTH_SETTINGS{
        .name = "width",
        .flag = 'w',
        .defaultValue = 640,
        .min = 100,
        .max = 4096
};

constexpr settings<uint32_t> HEIGHT_SETTINGS{
        .name = "height",
        .flag = 'h',
        .defaultValue = 480,
        .min = 100,
        .max = 4096
};


constexpr uint8_t TIME_TO_DISCONNECT{2};

constexpr uint8_t MAX_CLIENTS{25};

constexpr uint32_t CLIENT_MESSAGE_MIN_LENGTH{
        sizeof(uint64_t) + sizeof(uint8_t) + sizeof(uint32_t)
};

constexpr uint32_t CLIENT_MESSAGE_MAX_LENGTH{
        CLIENT_MESSAGE_MIN_LENGTH + sizeof(char[MAX_PLAYER_NAME_LENGTH])
};

#endif //CONSTANTS_H