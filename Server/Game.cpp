#include "Game.h"

#include <iostream>

#include "Server.h"


std::shared_ptr<Player> Game::newPlayer(std::string const &playerName, std::shared_ptr<Client> const &client) {
    if (playerName.empty()) {
        auto it = observers.insert(std::make_shared<Player>(Player{playerName, client}));
        return *it.first;
    } else {
        auto it = allPlayers.insert(std::make_shared<Player>(Player{playerName, client}));
        return *it.first;
    }
}


void Game::disconnectPlayer(std::shared_ptr<Client> const &client) {
    if (!client->hasPlayer()) {
        return;
    }

    auto observer = observers.find(client);

    if (observer != observers.end()) {
        log("Observer has been disconnected.");
        observers.erase(observer);
    }

    std::shared_ptr<Player> const clientPlayer{client->getPlayer()};

    auto player = allPlayers.find(clientPlayer);

    if (player != allPlayers.end()) {
        log("Player ", (*player)->getName(), " has been disconnected.");
        allPlayers.erase(player);

        if (readyPlayers.erase(clientPlayer) > 0) {
            log("All players: ", allPlayers.size(), ", ready players: ", readyPlayers.size(), ".");
        }
    }
}


std::shared_ptr<Player> Game::getPlayer(std::string const &playerName) const {
    auto it = allPlayers.find(playerName);
    return it != allPlayers.end() ? *it : nullptr;
}


void Game::playerMessage(std::shared_ptr<Client> const &client, uint8_t const turnDirection,
                         uint32_t const nextExpectedEventNo) {
    if (!client->hasPlayer()) {
        return;
    }

    std::shared_ptr<Player> const player{client->getPlayer()};

    if (!player->isObserver()) {
        player->setTurnDirection((Direction) turnDirection);
        if (!isRunning) {
            if (player->isReady() && readyPlayers.find(player) == readyPlayers.end()) {
                log("Player ", player->getName(), " is ready.");
                readyPlayers.insert(player);

                if (readyPlayers.size() == allPlayers.size() && readyPlayers.size() > 1) {
                    newGame = true;
                } else {
                    log("All players: ", allPlayers.size(), ", ready players: ", readyPlayers.size(), ".");
                }
            }
        }
    }

    for (uint32_t i = nextExpectedEventNo; i < events.size(); ++i) {
        eventsBuffer.reset();
        eventsBuffer << id;

        while (i < events.size() && eventsBuffer.getEmptySpace() >= events[i]->getAllDataSize()) {
            events[i]->write(eventsBuffer);
            ++i;
        }

        server.sendToPlayer(player, eventsBuffer);
    }

    eventsBuffer.reset();
}


void Game::start() {
    isRunning = true;
    gameOver = false;
    newGame = false;

    events.clear();
    inGamePlayers.clear();
    inGamePlayers.insert(inGamePlayers.begin(), allPlayers.begin(), allPlayers.end());
    readyPlayers.clear();
    board.reset();

    id = generator.getNext();

    logImportant("Game has started (id: ", id, ").");

    flushBuffer();

    generateEvent<NewGameEvent>(board.getWidth(), board.getHeight(), inGamePlayers);

    alivePlayers = inGamePlayers.size();

    for (uint8_t i = 0; i < inGamePlayers.size(); ++i) {
        std::shared_ptr<Player> &player = inGamePlayers[i];

        double x = generator.getNext() % board.getWidth() + 0.5;
        double y = generator.getNext() % board.getHeight() + 0.5;
        uint32_t direction = generator.getNext() % 360;

        player->newGame({x, y}, direction);

        Pixel pixel = player->getPixel();
        if (!board.eat(pixel)) {
            eliminatePlayer(i, player);
        } else {
            generateEvent<PixelEvent>(pixel, i);
        }
    }

    if (gameOver) {
        end();
    }

    flushBuffer();
}


void Game::nextTurn() {
    if (isRunning) {
        for (uint32_t i = 0; i < inGamePlayers.size(); ++i) {
            std::shared_ptr<Player> &player = inGamePlayers[i];

            if (player->isAlive()) {
                player->move(turningSpeed);

                Pixel pixel = player->getPixel();
                if (player->changedPixel()) {
                    if (!board.isInside(pixel) || !board.eat(pixel)) {
                        eliminatePlayer(i, player);
                    } else {
                        generateEvent<PixelEvent>(pixel, i);
                    }
                }
            }
        }

        if (gameOver) {
            end();
        }

        flushBuffer();
    } else {
        if (newGame) {
            start();
        }
    }
}


void Game::eliminatePlayer(uint8_t const playerId, std::shared_ptr<Player> const &player) {
    generateEvent<PlayerEliminatedEvent>(playerId);
    player->setAlive(false);
    --alivePlayers;

    log("Player ", player->getName(), " has died.");

    if (alivePlayers <= 1) {
        gameOver = true;
    }
}


void Game::end() {
    logImportant("Game over.");
    generateEvent<GameOverEvent>();
    isRunning = false;

    for (auto const &player : allPlayers) {
        player->gameOver();
    }
    for (auto const &player : observers) {
        player->gameOver();
    }
}


void Game::flushBuffer() {
    if (eventsBuffer.getSize() == 0) {
        eventsBuffer << id;
        return;
    } else if (eventsBuffer.getSize() == sizeof(id)) {
        return;
    }

    for (auto const &player : allPlayers) {
        server.sendToPlayer(player, eventsBuffer);
    }
    for (auto const &player : observers) {
        server.sendToPlayer(player, eventsBuffer);
    }

    eventsBuffer.reset();
    eventsBuffer << id;
}