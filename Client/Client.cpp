#include "Client.h"

#include <iostream>

#include "../Common/logger.h"

#include "GuiMessage.h"


[[noreturn]] void Client::run() {
    logImportant("Client session: ", sessionId, ".");

    timer.start(MESSAGE_INTERVAL);

    while (true) {
        struct epoll_event &event = epoll.wait();

        if (event.events & EPOLLIN) {
            if (event.data.fd == server.getDescriptor()) {
                receiveMessage();
            } else if (event.data.fd == gui.getDescriptor()) {
                receiveAction();
            } else if (event.data.fd == timer.getDescriptor()) {
                sendMessage();
            } else {
                fatalError("Invalid descriptor.");
            }
        }
    }
}


void Client::sendMessage() const {
    timer.unmark();

    OutputBuffer buffer;
    buffer << sessionId << player.getTurnDirection()
           << parser.getNextEventNo();

    if (!player.getName().empty()) {
        buffer << player.getName();
    }

    server.send(buffer);
}


void Client::receiveMessage() {
    InputBuffer buffer{server.read()};
    parser.parse(buffer);
}


void Client::receiveAction() {
    std::string message = gui.getLine();

    if (message.empty()) {
        fatalError("GUI connection closed.");
    }

    player.guiAction(message);
}


void Client::sendMessageToGui(std::string const &message) const {
    gui.write(message);
}


void Client::newGame(uint32_t const maxx, uint32_t const maxy, std::vector<std::string> const &playersNames) {
    game.newGame(maxx, maxy, playersNames);

    logImportant("New game (id: ", game.getId(), ").");
    log("Players: ");

    for (auto const &playerName : game.getPlayers()) {
        log(playerName);
    }

    sendMessageToGui(
            newGameMessage(maxx, maxy, game.getPlayers())
    );
}


void Client::pixel(uint32_t const x, uint32_t const y, uint8_t const playerIndex) {
    if (!game.isPlayerValid(playerIndex)) {
        fatalError("Invalid player id.");
        return;
    }

    if (!game.isPixelValid(x, y)) {
        fatalError("Invalid pixel.");
        return;
    }

    sendMessageToGui(
            pixelMessage(x, y, game.getPlayer(playerIndex))
    );
}


void Client::playerEliminated(uint8_t const playerIndex) {
    if (!game.isPlayerValid(playerIndex)) {
        fatalError("Invalid player id.");
        return;
    }

    log("Player ", game.getPlayer(playerIndex), " has died.");

    sendMessageToGui(
            playerEliminatedMessage(game.getPlayer(playerIndex))
    );
}


void Client::gameOver() {
    logImportant("Game over.");

    player.gameOver();
}
