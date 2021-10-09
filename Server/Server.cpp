#include "Server.h"

#include <iostream>
#include <cmath>

/*
 * Function checks if player name is valid.
 */
static inline bool isNameValid(std::string const &name) {
    return std::all_of(name.begin(), name.end(), [](char const c) -> bool {
        return MIN_NAME_CHAR <= c && c <= MAX_NAME_CHAR;
    });
}


[[noreturn]] void Server::run() {
    timer.start(std::floor(1000. / roundsPerSecond));

    while (true) {
        struct epoll_event &event = epoll.wait();

        if (event.events & EPOLLIN) {
            if (event.data.fd == socket.getDescriptor()) {
                receiveMessage();
            } else if (event.data.fd == timer.getDescriptor()) {
                newRound();
            } else {
                fatalError("Invalid descriptor");
            }
        }
    }
}


void Server::sendToPlayer(std::shared_ptr<Player> const &player, OutputBuffer const &buffer) const {
    auto it = clients.find(player);

    if (it != clients.end()) {
        (*it)->send(socket, buffer);
    }
}


void Server::newRound() {
    timer.unmark();

    std::vector<std::shared_ptr<Client>> toDisconnect;

    for (auto const &client : clients) {
        if (client->lastMessage + roundsToDisconnect < round) {
            toDisconnect.push_back(client);
        }
    }

    for (auto const &client : toDisconnect) {
        if (!client->hasPlayer()) {
            log("Timeout: ", client->getPlayer()->getName(), ".");
        }

        game.disconnectPlayer(client);
        clients.erase(client);
    }

    ++round;
    game.nextTurn();
}


void Server::receiveMessage() {
    uint64_t sessionId;
    uint8_t turnDirection;
    uint32_t nextExpectedEventNo;

    sockaddr_in6 clientAddress{};
    socklen_t addressLength{sizeof(clientAddress)};

    socket.receive(socketBuffer, (struct sockaddr *) &clientAddress, &addressLength);

    if (socketBuffer.remaining() < CLIENT_MESSAGE_MIN_LENGTH ||
        socketBuffer.remaining() > CLIENT_MESSAGE_MAX_LENGTH) {
        logError("Invalid message.");
        return;
    }

    socketBuffer >> sessionId >> turnDirection >> nextExpectedEventNo;

    std::string playerName = socketBuffer.readAll();

    if (!isNameValid(playerName)) {
        logError("Invalid player name.");
        return;
    }

    executeMessage(clientAddress, sessionId, turnDirection, nextExpectedEventNo, playerName);
}


void Server::executeMessage(sockaddr_in6 const &clientAddress, uint32_t sessionId,
                            uint8_t turnDirection, uint32_t nextExpectedEventNo,
                            std::string const &playerName) {
    std::shared_ptr<Player> player{game.getPlayer(playerName)};

    auto it = clients.find(clientAddress);
    std::shared_ptr<Client> client;
    bool newPlayer;

    if (it == clients.end()) {
        if (player != nullptr) {
            logError("Username ", player->getName(), " is already taken.");
            return;
        } else if (clients.size() >= MAX_CLIENTS) {
            logError("Too many clients.");
            return;
        }

        client = std::make_shared<Client>(clientAddress);
        clients.insert(client);
        client->sessionId = sessionId;
        newPlayer = true;
    } else {
        client = *it;

        if (player != nullptr && client != player->getClient()) {
            logError("Username ", player->getName(), " is already taken.");
            return;
        }

        if (sessionId < client->sessionId) {
            logError("Invalid session id (too old).");
            return;
        }

        newPlayer = client->sessionId != sessionId;
    }

    if (newPlayer) {
        createPlayer(client, playerName);
    }

    client->lastMessage = round;
    game.playerMessage(client, turnDirection, nextExpectedEventNo);
}


void Server::createPlayer(std::shared_ptr<Client> const &client, std::string const &playerName) {
    if (playerName.empty()) {
        log("New observer.");
    } else {
        log("New player ", playerName, ".");
    }

    if (!client->hasPlayer()) {
        game.disconnectPlayer(client);
        client->disconnect();
    }

    client->setPlayer(game.newPlayer(playerName, client));
}