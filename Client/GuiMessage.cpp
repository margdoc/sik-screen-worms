#include "GuiMessage.h"

std::string newGameMessage(uint32_t const maxx, uint32_t const maxy,
                           std::vector<std::string> const &players) {
    std::string message = "NEW_GAME " + std::to_string(maxx) + " " + std::to_string(maxy);

    for (auto const &player : players) {
        message += " " + player;
    }

    return message + "\n";
}


std::string pixelMessage(uint32_t const x, uint32_t const y, std::string const &player) {
    return "PIXEL " + std::to_string(x) + " " + std::to_string(y) + " " + player + "\n";
}


std::string playerEliminatedMessage(std::string const &player) {
    return "PLAYER_ELIMINATED " + player + "\n";
}