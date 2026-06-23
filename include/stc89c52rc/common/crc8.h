#ifndef STC89C52RC_COMMON_CRC8_H
#define STC89C52RC_COMMON_CRC8_H

#include <stdint.h>

#define STC_CRC8_INITIAL 0x00u
#define STC_CRC8_POLY 0x07u

uint8_t stc_crc8_update(uint8_t crc, uint8_t data);
uint8_t stc_crc8(const uint8_t *data, uint8_t length);

#endif
