#ifndef EVENT_H
#define EVENT_H

#include <utility>
#include <cstdint>
#include <string>
#include <vector>

#include "../Common/Buffer.h"

#include "geometry.h"
#include "Player.h"

/*
 * Enum of all events types.
 */
enum EventType {
    NEW_GAME = 0,
    PIXEL = 1,
    PLAYER_ELIMINATED = 2,
    GAME_OVER = 3,
};

/*
 * Abstract class for handling event data.
 */
class Event {
public:
    explicit Event(EventType const type, uint32_t const number)
            : type{(uint8_t) type}, number{number} {}

    virtual ~Event() = default;

    /*
     * Methods returns event_data field length.
     */
    [[nodiscard]] virtual uint32_t getSize() const = 0;

    /*
     * Method returns all event fields length.
     */
    [[nodiscard]] uint32_t getAllDataSize() const;

    /*
     * Method write all event fields event to buffer.
     */
    void write(OutputBuffer &buffer) const;

private:
    uint8_t const type;
    uint32_t const number;

protected:
    /*
     * Method returns len, event_no and event_type fields length.
     */
    [[nodiscard]] uint32_t getMetaDataSize() const;

    /*
     * Method writes event_data field to buffer.
     */
    virtual void writeEventData(OutputBuffer &buffer) const = 0;
};

/*
 * Classes representing each event type.
 */

class NewGameEvent : public Event {
public:
    NewGameEvent(uint32_t const number, uint32_t const maxx, uint32_t const maxy,
                 std::vector<std::shared_ptr<Player>> players)
            : Event{EventType::NEW_GAME, number}, maxx{maxx}, maxy{maxy}, players{std::move(players)} {}

    [[nodiscard]] uint32_t getSize() const override;

protected:
    void writeEventData(OutputBuffer &buffer) const override;

    uint32_t const maxx, maxy;
    std::vector<std::shared_ptr<Player>> const players;
};


class PixelEvent : public Event {
public:
    PixelEvent(uint32_t const number, Pixel const &pixel, uint8_t const player)
            : Event{EventType::PIXEL, number}, x{(uint32_t) pixel.x}, y{(uint32_t) pixel.y}, player{player} {}

    [[nodiscard]] uint32_t getSize() const override;

protected:
    void writeEventData(OutputBuffer &buffer) const override;

    uint32_t const x, y;
    uint8_t const player;
};


class PlayerEliminatedEvent : public Event {
public:
    explicit PlayerEliminatedEvent(uint32_t const number, uint8_t const player)
            : Event{EventType::PLAYER_ELIMINATED, number}, player{player} {}

    [[nodiscard]] uint32_t getSize() const override;

protected:
    void writeEventData(OutputBuffer &buffer) const override;

    uint8_t const player;
};


class GameOverEvent : public Event {
public:
    explicit GameOverEvent(uint32_t const number)
            : Event{EventType::GAME_OVER, number} {}

    [[nodiscard]] uint32_t getSize() const override;

protected:
    void writeEventData(OutputBuffer &buffer) const override;
};

#endif //EVENT_H
