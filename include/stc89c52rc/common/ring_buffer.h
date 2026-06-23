#ifndef STC89C52RC_COMMON_RING_BUFFER_H
#define STC89C52RC_COMMON_RING_BUFFER_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t *data;
    uint8_t capacity;
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} stc_ring_buffer_t;

bool stc_ring_buffer_init(stc_ring_buffer_t *buffer, uint8_t *storage, uint8_t capacity);
void stc_ring_buffer_clear(stc_ring_buffer_t *buffer);
bool stc_ring_buffer_push(stc_ring_buffer_t *buffer, uint8_t value);
bool stc_ring_buffer_pop(stc_ring_buffer_t *buffer, uint8_t *value);
bool stc_ring_buffer_peek(const stc_ring_buffer_t *buffer, uint8_t *value);
uint8_t stc_ring_buffer_size(const stc_ring_buffer_t *buffer);
uint8_t stc_ring_buffer_free(const stc_ring_buffer_t *buffer);
bool stc_ring_buffer_is_empty(const stc_ring_buffer_t *buffer);
bool stc_ring_buffer_is_full(const stc_ring_buffer_t *buffer);

#endif
