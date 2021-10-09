#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include "../Common/Buffer.h"


class Client;

/*
 * Class for parsing messages from server.
 */
class MessageParser {
public:
    explicit MessageParser(Client &client)
            : client{client}, nextExpectedEventNo{0} {}

    /*
     * Method parses message from server.
     */
    void parse(InputBuffer &buffer);

    [[nodiscard]] uint32_t getNextEventNo() const {
        return nextExpectedEventNo;
    }

private:
    Client &client;
    uint32_t nextExpectedEventNo;

    /*
     * Method reads player name from buffer.
     */
    static std::string readPlayerName(InputBuffer &buffer, uint32_t &remaining);

    /*
     * Method reads new game event from buffer.
     */
    void parseNewGame(InputBuffer &buffer, uint32_t length, bool execute);

    /*
     * Method reads pixel event from buffer.
     */
    void parsePixel(InputBuffer &buffer, uint32_t length, bool execute);

    /*
     * Method reads player eliminated event from buffer.
     */
    void parsePlayerEliminated(InputBuffer &buffer, uint32_t length, bool execute);

    /*
     * Method reads game over event from buffer.
     */
    void parseGameOver(InputBuffer &buffer, uint32_t length, bool execute);

    using parseMethod = void (MessageParser::*)(InputBuffer&, uint32_t, bool);

    parseMethod parseEventsTable[4] {
            &MessageParser::parseNewGame,
            &MessageParser::parsePixel,
            &MessageParser::parsePlayerEliminated,
            &MessageParser::parseGameOver
    };
};

#endif //MESSAGEPARSER_H
