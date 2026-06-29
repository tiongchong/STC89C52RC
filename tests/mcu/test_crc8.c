#include "stc89c52rc/cli/cli_print.h"
#include "stc89c52rc/cli/test_utils.h"
#include "stc89c52rc/common/crc8.h"

/**
 * CRC8 computation test
 * Usage: test crc8.compute [data=<hex_bytes>]
 */
int test_crc8(int argc, char *argv[]) __reentrant
{
    const char *data_str = test_arg_value(argc, argv, "data");
    
    if (!data_str) {
        // Test with default pattern
        uint8_t test_data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
        uint8_t crc = stc_crc8(test_data, 5);
        cli_printf("CRC8 of [01 02 03 04 05]: 0x%02X\r\n", crc);
    } else {
        // Parse hex string and compute
        cli_printf("CRC8 of '%s': parsing not implemented in this test\r\n", data_str);
    }
    
    // Test the incremental computation
    uint8_t test_data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint8_t crc_bulk = stc_crc8(test_data, 5);
    
    uint8_t crc_inc = 0;
    uint8_t i;
    for (i = 0; i < 5; i++) {
        crc_inc = stc_crc8_update(crc_inc, test_data[i]);
    }
    
    cli_printf("Bulk CRC: 0x%02X, Incremental CRC: 0x%02X\r\n", crc_bulk, crc_inc);
    
    return (crc_bulk == crc_inc) ? test_pass() : test_fail();
}
