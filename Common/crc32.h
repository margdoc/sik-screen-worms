#ifndef CRC32_H
#define CRC32_H

#include <cstdint>

/*
 * Function calculates crc32 value.
 */
uint32_t crc32(char const *data, uint32_t length);

#endif //CRC32_H
