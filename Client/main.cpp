#include <getopt.h>

#include "../Common/logger.h"

#include "Client.h"


/*
 * Function checks if port argument is correct number.
 */
static void checkPortValue(std::string const &option) {
    for (char const c : option) {
        if (c < '0' || '9' < c) {
            fatalError("Invalid port argument: ", option);
        }
    }

    if (option[0] == '0') {
        fatalError("Invalid port argument: ", option);
    }

    uint64_t value = std::stoull(option);

    if (UINT32_MAX < value) {
        fatalError("Invalid port argument: ", option);
    }
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fatalError("Usage: ", argv[0], " game_server [-n player_name] [-p n] [-i gui_server] [-r n]");
    }

    char const *serverHost{argv[1]};
    char const *serverPort{DEFAULT_SERVER_PORT};
    char const *guiHost{DEFAULT_GUI_HOST};
    char const *guiPort{DEFAULT_GUI_PORT};
    char const *playerName{DEFAULT_PLAYER_NAME};

    int flag;

    while ((flag = getopt(argc - 1, argv + 1, "n:p:i:r:")) != -1) {
        switch (flag) {
            case 'n':
                playerName = optarg;
                break;
            case 'p':
                serverPort = optarg;
                break;
            case 'i':
                guiHost = optarg;
                break;
            case 'r':
                guiPort = optarg;
                break;
            default:
                fatalError("Usage: ", argv[0], " game_server [-n player_name] [-p n] [-i gui_server] [-r n]");
        }
    }

    checkPortValue(serverPort);
    checkPortValue(guiPort);

    Client client{playerName, serverHost, serverPort, guiHost, guiPort};

    client.run();
}