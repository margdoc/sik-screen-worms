#include "Client.h"


void Client::disconnect() {
    if (player != std::nullopt) {
        getPlayer()->disconnect();
        player = std::nullopt;
    }
}