#include "RandomGenerator.h"


uint32_t RandomGenerator::getNext() {
    uint32_t const previous = next;
    next = (static_cast<uint64_t>(previous) * STEP) % MOD;

    return previous;
}
