#include <array>
#include <cstdint>

#include <gtest/gtest.h>

#include <stc89c52rc/common/crc8.h>

TEST(Crc8Test, MatchesKnownVector)
{
    const std::array<std::uint8_t, 9> vector = {
        '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };

    EXPECT_EQ(stc_crc8(vector.data(), static_cast<std::uint8_t>(vector.size())), 0xf4u);
}

TEST(Crc8Test, IncrementalUpdateMatchesBulkCalculation)
{
    const std::array<std::uint8_t, 4> data = { 0x12u, 0x34u, 0x56u, 0x78u };
    std::uint8_t crc = STC_CRC8_INITIAL;

    for (std::uint8_t value : data) {
        crc = stc_crc8_update(crc, value);
    }

    EXPECT_EQ(crc, stc_crc8(data.data(), static_cast<std::uint8_t>(data.size())));
}
