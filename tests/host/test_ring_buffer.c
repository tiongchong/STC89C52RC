#include <stdint.h>
#include <stc89c52rc/common/ring_buffer.h>
#include "minitest.h"

void test_ring_buffer_push_pop_order(void)
{
    uint8_t storage[3];
    uint8_t value = 0u;
    stc_ring_buffer_t buffer;

    MT_ASSERT(stc_ring_buffer_init(&buffer, storage, (uint8_t)sizeof(storage)));
    MT_ASSERT(stc_ring_buffer_push(&buffer, 0x11u));
    MT_ASSERT(stc_ring_buffer_push(&buffer, 0x22u));
    MT_ASSERT(stc_ring_buffer_size(&buffer) == 2u);
    MT_ASSERT(stc_ring_buffer_pop(&buffer, &value));
    MT_ASSERT(value == 0x11u);
    MT_ASSERT(stc_ring_buffer_pop(&buffer, &value));
    MT_ASSERT(value == 0x22u);
    MT_ASSERT(stc_ring_buffer_is_empty(&buffer));
}

void test_ring_buffer_wrap_and_full(void)
{
    uint8_t storage[2];
    uint8_t value = 0u;
    stc_ring_buffer_t buffer;

    MT_ASSERT(stc_ring_buffer_init(&buffer, storage, (uint8_t)sizeof(storage)));
    MT_ASSERT(stc_ring_buffer_push(&buffer, 0xaau));
    MT_ASSERT(stc_ring_buffer_push(&buffer, 0xbbu));
    MT_ASSERT(stc_ring_buffer_is_full(&buffer));
    MT_ASSERT(!stc_ring_buffer_push(&buffer, 0xccu));
    MT_ASSERT(stc_ring_buffer_pop(&buffer, &value));
    MT_ASSERT(value == 0xaau);
    MT_ASSERT(stc_ring_buffer_push(&buffer, 0xccu));
    MT_ASSERT(stc_ring_buffer_pop(&buffer, &value));
    MT_ASSERT(value == 0xbbu);
    MT_ASSERT(stc_ring_buffer_pop(&buffer, &value));
    MT_ASSERT(value == 0xccu);
}

void test_ring_buffer_rejects_invalid_init(void)
{
    uint8_t storage[1];
    stc_ring_buffer_t buffer;

    MT_ASSERT(!stc_ring_buffer_init(0, storage, (uint8_t)sizeof(storage)));
    MT_ASSERT(!stc_ring_buffer_init(&buffer, 0, (uint8_t)sizeof(storage)));
    MT_ASSERT(!stc_ring_buffer_init(&buffer, storage, 0u));
}
