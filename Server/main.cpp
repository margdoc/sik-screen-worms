#include <getopt.h>
#include <iostream>
#include <netdb.h>

#include "Server.h"

/*
 * Function checks if flag argument is correct number.
 */
static uint32_t getOptionValue(std::string const &option) {
    for (char const c : option) {
        if (c < '0' || '9' < c) {
            fatalError("Invalid option argument: ", option);
        }
    }

    if (option[0] == '0') {
        fatalError("Invalid option argument: ", option);
    }

    uint64_t value = std::stoull(option);

    if (UINT32_MAX < value) {
        fatalError("Invalid option argument: ", option);
    }

    return static_cast<uint32_t>(value);
}

/*
 * Macro for reading and checking correctness of flag argument.
 */
#define flagCase(variable, settings)                                        \
    case settings.flag:                                                     \
        (variable) = getOptionValue(optarg);                                \
        if ((variable) < (settings).min || (settings).max < (variable)) {   \
            fatalError("Invalid ", (settings).name, ": ", variable);        \
        }                                                                   \
        break;


int main(int argc, char *argv[]) {
    uint32_t seed{static_cast<uint32_t>(time(nullptr))};
    uint32_t port{PORT_SETTINGS.defaultValue};
    uint32_t turningSpeed{TURNING_SPEED_SETTINGS.defaultValue};
    uint32_t roundsPerSecond{ROUNDS_PER_SECOND_SETTINGS.defaultValue};
    uint32_t width{WIDTH_SETTINGS.defaultValue};
    uint32_t height{HEIGHT_SETTINGS.defaultValue};

    int flag;

    while ((flag = getopt(argc, argv, "p:s:t:v:w:h:")) != -1) {
        switch (flag) {
            case 's':
                seed = getOptionValue(optarg);
                break;
            flagCase(port, PORT_SETTINGS)
            flagCase(turningSpeed, TURNING_SPEED_SETTINGS)
            flagCase(roundsPerSecond, ROUNDS_PER_SECOND_SETTINGS)
            flagCase(width, WIDTH_SETTINGS)
            flagCase(height, HEIGHT_SETTINGS)
            default:
                fatalError("Usage: ", argv[0], " [-p n] [-s n] [-t n] [-v n] [-w n] [-h n]");
        }
    }

    // Checking if occurred something else than flags.
    for (int i = argc - 1; i > 0; --i) {
        if (argv[i][0] != '-' && argv[i - 1][0] != '-') {
            fatalError("Invalid option: ", argv[i]);
        } else if (argv[i][0] == '-') {
            break;
        }
    }


    Server server{port, roundsPerSecond, GameSettings{seed, turningSpeed, width, height}};

    server.run();
}