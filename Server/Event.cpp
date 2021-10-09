#include "Event.h"

#include "../Common/crc32.h"


uint32_t Event::getMetaDataSize() const {
    return sizeof(number) + sizeof(type);
}


uint32_t Event::getAllDataSize() const {
    return getSize() + 2 * sizeof(uint32_t);
}


void Event::write(OutputBuffer &buffer) const {
    char const *const firstEmpty = reinterpret_cast<char const*>(buffer.getNextEmpty());
    buffer << getSize() << number << type;

    writeEventData(buffer);

    uint32_t crc32Value{crc32(firstEmpty, getSize() + sizeof(uint32_t))};
    buffer << crc32Value;
}


uint32_t NewGameEvent::getSize() const {
    uint32_t playersNamesLength = players.size();

    for (auto const &player : players) {
        playersNamesLength += player->getName().size();
    }

    return getMetaDataSize() + sizeof(maxx) + sizeof(maxy) + playersNamesLength;
}


void NewGameEvent::writeEventData(OutputBuffer &buffer) const {
    buffer << maxx << maxy;

    for (auto const &player : players) {
        std::string const playerName = player->getName();
        buffer << playerName << '\0';
    }
}


uint32_t PixelEvent::getSize() const {
    return getMetaDataSize() + sizeof(player) + sizeof(x) + sizeof(y);
}


void PixelEvent::writeEventData(OutputBuffer &buffer) const {
    buffer << player << x << y;
}


uint32_t PlayerEliminatedEvent::getSize() const {
    return getMetaDataSize() + sizeof(player);
}


void PlayerEliminatedEvent::writeEventData(OutputBuffer &buffer) const {
    buffer << player;
}


uint32_t GameOverEvent::getSize() const {
    return getMetaDataSize();
}


void GameOverEvent::writeEventData([[maybe_unused]] OutputBuffer &buffer) const {
    // Empty.
}