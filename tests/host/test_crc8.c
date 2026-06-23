#include <stdint.h>
#include <stc89c52rc/common/crc8.h>
#include "minitest.h"

void test_crc8_known_vector(void)
{
    static const uint8_t vector[] = {
        '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };

    MT_ASSERT(stc_crc8(vector, (uint8_t)sizeof(vector)) == 0xf4u);
}

void test_crc8_incremental_matches_bulk(void)
{
    static const uint8_t data[] = { 0x12u, 0x34u, 0x56u, 0x78u };
    uint8_t crc = STC_CRC8_INITIAL;
    uint8_t index;

    for (index = 0u; index < sizeof(data); index++) {
        crc = stc_crc8_update(crc, data[index]);
    }

    MT_ASSERT(crc == stc_crc8(data, (uint8_t)sizeof(data)));
}
