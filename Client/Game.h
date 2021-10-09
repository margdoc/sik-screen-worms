#ifndef GAME_H
#define GAME_H

#include <cstdint>
#include <utility>
#include <vector>
#include <string>
#include <set>

/*
 * Class for handling game data and operations.
 */
class Game {
public:
    Game()
            : id{0}, width{}, height{} {}

    [[nodiscard]] bool isPlayerValid(uint8_t const player) const {
        return player < players.size();
    }

    [[nodiscard]] std::string getPlayer(uint8_t const player) const {
        return players[player];
    }

    [[nodiscard]] std::vector<std::string> const &getPlayers() const {
        return players;
    }

    [[nodiscard]] bool isPixelValid(uint32_t const x, uint32_t const y) const {
        return x < width && y < height;
    }

    [[nodiscard]] uint32_t getId() const {
        return id;
    }

    void setId(uint32_t const newId) {
        previousIds.insert(id);
        id = newId;
    }

    /*
     * Method checks if game id occurde before.
     */
    [[nodiscard]] bool occuredBefore(uint32_t const newId) const {
        return previousIds.find(newId) != previousIds.end();
    }

    void newGame(uint32_t const newWidth, uint32_t const newHeight,
                 std::vector<std::string> newPlayers) {
        width = newWidth;
        height = newHeight;
        players = std::move(newPlayers);
    }

private:
    uint32_t id;
    std::set<uint32_t> previousIds;
    std::vector<std::string> players;
    uint32_t width, height;
};

#endif //GAME_H
