#ifndef GUIMESSAGE_H
#define GUIMESSAGE_H

#include <string>
#include <vector>

/*
 * File contains functions used to generate string messages for gui.
 */

std::string newGameMessage(uint32_t maxx, uint32_t maxy,
                           std::vector<std::string> const &players);

std::string pixelMessage(uint32_t x, uint32_t y, std::string const &player);

std::string playerEliminatedMessage(std::string const &player);

#endif //GUIMESSAGE_H
