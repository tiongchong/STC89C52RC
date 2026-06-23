#include <array>
#include <cstdint>

#include <gtest/gtest.h>

#include <stc89c52rc/common/ring_buffer.h>

TEST(RingBufferTest, PushPopPreservesOrder)
{
    std::array<std::uint8_t, 3> storage = {};
    std::uint8_t value = 0u;
    stc_ring_buffer_t buffer = {};

    ASSERT_TRUE(stc_ring_buffer_init(&buffer, storage.data(), static_cast<std::uint8_t>(storage.size())));
    EXPECT_TRUE(stc_ring_buffer_push(&buffer, 0x11u));
    EXPECT_TRUE(stc_ring_buffer_push(&buffer, 0x22u));
    EXPECT_EQ(stc_ring_buffer_size(&buffer), 2u);

    ASSERT_TRUE(stc_ring_buffer_pop(&buffer, &value));
    EXPECT_EQ(value, 0x11u);
    ASSERT_TRUE(stc_ring_buffer_pop(&buffer, &value));
    EXPECT_EQ(value, 0x22u);
    EXPECT_TRUE(stc_ring_buffer_is_empty(&buffer));
}

TEST(RingBufferTest, WrapsAndRejectsPushWhenFull)
{
    std::array<std::uint8_t, 2> storage = {};
    std::uint8_t value = 0u;
    stc_ring_buffer_t buffer = {};

    ASSERT_TRUE(stc_ring_buffer_init(&buffer, storage.data(), static_cast<std::uint8_t>(storage.size())));
    EXPECT_TRUE(stc_ring_buffer_push(&buffer, 0xaau));
    EXPECT_TRUE(stc_ring_buffer_push(&buffer, 0xbbu));
    EXPECT_TRUE(stc_ring_buffer_is_full(&buffer));
    EXPECT_FALSE(stc_ring_buffer_push(&buffer, 0xccu));

    ASSERT_TRUE(stc_ring_buffer_pop(&buffer, &value));
    EXPECT_EQ(value, 0xaau);
    EXPECT_TRUE(stc_ring_buffer_push(&buffer, 0xccu));
    ASSERT_TRUE(stc_ring_buffer_pop(&buffer, &value));
    EXPECT_EQ(value, 0xbbu);
    ASSERT_TRUE(stc_ring_buffer_pop(&buffer, &value));
    EXPECT_EQ(value, 0xccu);
}

TEST(RingBufferTest, RejectsInvalidInit)
{
    std::array<std::uint8_t, 1> storage = {};
    stc_ring_buffer_t buffer = {};

    EXPECT_FALSE(stc_ring_buffer_init(nullptr, storage.data(), static_cast<std::uint8_t>(storage.size())));
    EXPECT_FALSE(stc_ring_buffer_init(&buffer, nullptr, static_cast<std::uint8_t>(storage.size())));
    EXPECT_FALSE(stc_ring_buffer_init(&buffer, storage.data(), 0u));
}
