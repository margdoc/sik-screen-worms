#include "MessageParser.h"

#include <iostream>

#include "../Common/crc32.h"
#include "../Common/constants.h"
#include "../Common/logger.h"

#include "Client.h"
#include "constants.h"


void MessageParser::parse(InputBuffer &buffer) {
    uint32_t gameId;
    uint32_t length;
    uint32_t number;
    uint8_t type;

    buffer >> gameId;

    Game &game = client.getGame();

    if (gameId != game.getId() && !game.occuredBefore(gameId)) {
        game.setId(gameId);
        nextExpectedEventNo = 0;
    }

    while (buffer.remaining() > 0) {
        char const *messageBeginning{reinterpret_cast<char const *>(buffer.getNextEmpty())};
        buffer >> length >> number >> type;

        uint32_t crc32Value;
        uint32_t crc32Position = length + sizeof(length);

        memcpy(&crc32Value, messageBeginning + crc32Position, sizeof(crc32Value));
        crc32Value = bigEndianToHost(crc32Value);
        uint32_t const controlSum{crc32(messageBeginning, length + sizeof(length))};

        if (crc32Value != controlSum) {
            logError("Invalid control sum.");
            return;
        }

        if (type > MAX_EVENT_TYPE) {
            buffer.skip(length - EVENT_META_DATA_LENGTH + sizeof(crc32Value));
            logError("Invalid event type (", type, ").");
            continue;
        }

        bool const execute = number == nextExpectedEventNo;

        if (!execute) {
            logError("Next expected event: ", nextExpectedEventNo, ", got: ", number, ".");
            return;
        }

        nextExpectedEventNo += execute;
        (this->*parseEventsTable[type])(buffer, length, execute);

        buffer.skip(sizeof(crc32Value));
    }
}


void MessageParser::parseNewGame(InputBuffer &buffer, uint32_t const length, bool const execute) {
    uint32_t remaining = length - EVENT_META_DATA_LENGTH - sizeof(uint32_t) - sizeof(uint32_t);

    if (execute) {
        uint32_t maxx;
        uint32_t maxy;
        buffer >> maxx >> maxy;

        std::vector<std::string> players;

        while (remaining > 0) {
            std::string playerName = readPlayerName(buffer, remaining);
            players.push_back(playerName);
        }

        client.newGame(maxx, maxy, players);
    } else {
        buffer.skip(length - EVENT_META_DATA_LENGTH);
    }
}


void MessageParser::parsePixel(InputBuffer &buffer, uint32_t const length, bool const execute) {
    if (execute) {
        uint8_t player;
        uint32_t x;
        uint32_t y;
        buffer >> player >> x >> y;

        client.pixel(x, y, player);
    } else {
        buffer.skip(length - EVENT_META_DATA_LENGTH);
    }
}


void MessageParser::parsePlayerEliminated(InputBuffer &buffer, uint32_t const length, bool const execute) {
    if (execute) {
        uint8_t player;
        buffer >> player;

        client.playerEliminated(player);
    } else {
        buffer.skip(length - EVENT_META_DATA_LENGTH);
    }
}


void MessageParser::parseGameOver([[maybe_unused]] InputBuffer &buffer,
                                  [[maybe_unused]] uint32_t const length,
                                  bool const execute) {
    if (execute) {
        client.gameOver();
    }
}


std::string MessageParser::readPlayerName(InputBuffer &buffer, uint32_t &remaining) {
    std::string playerName;
    buffer >> playerName;

    remaining -= playerName.size() + 1;
    return playerName;
}
