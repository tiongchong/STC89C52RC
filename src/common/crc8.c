#include <stc89c52rc/common/crc8.h>

uint8_t stc_crc8_update(uint8_t crc, uint8_t data)
{
    uint8_t bit;

    crc ^= data;
    for (bit = 0u; bit < 8u; bit++) {
        if ((crc & 0x80u) != 0u) {
            crc = (uint8_t)((crc << 1u) ^ STC_CRC8_POLY);
        } else {
            crc = (uint8_t)(crc << 1u);
        }
    }

    return crc;
}

uint8_t stc_crc8(const uint8_t *data, uint8_t length)
{
    uint8_t crc = STC_CRC8_INITIAL;
    uint8_t index;

    for (index = 0u; index < length; index++) {
        crc = stc_crc8_update(crc, data[index]);
    }

    return crc;
}
