#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <cstdint>

/*
 * Class for deterministic generating random numbers.
 */
class RandomGenerator {
public:
    explicit RandomGenerator(uint32_t const seed) : next{seed} {}

    /*
     * Method generates and returns next random number.
     */
    uint32_t getNext();
private:
    uint32_t next;

    // Algorithm constants.
    constexpr static uint64_t STEP {279410273};
    constexpr static uint64_t MOD  {4294967291};
};

#endif //RANDOMGENERATOR_H
