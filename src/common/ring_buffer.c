#include <stddef.h>
#include <stc89c52rc/common/ring_buffer.h>

bool stc_ring_buffer_init(stc_ring_buffer_t *buffer, uint8_t *storage, uint8_t capacity)
{
    if ((buffer == NULL) || (storage == NULL) || (capacity == 0u)) {
        return false;
    }

    buffer->data = storage;
    buffer->capacity = capacity;
    stc_ring_buffer_clear(buffer);
    return true;
}

void stc_ring_buffer_clear(stc_ring_buffer_t *buffer)
{
    if (buffer == NULL) {
        return;
    }

    buffer->head = 0u;
    buffer->tail = 0u;
    buffer->count = 0u;
}

bool stc_ring_buffer_push(stc_ring_buffer_t *buffer, uint8_t value)
{
    if ((buffer == NULL) || stc_ring_buffer_is_full(buffer)) {
        return false;
    }

    buffer->data[buffer->head] = value;
    buffer->head = (uint8_t)((buffer->head + 1u) % buffer->capacity);
    buffer->count++;
    return true;
}

bool stc_ring_buffer_pop(stc_ring_buffer_t *buffer, uint8_t *value)
{
    if ((buffer == NULL) || (value == NULL) || stc_ring_buffer_is_empty(buffer)) {
        return false;
    }

    *value = buffer->data[buffer->tail];
    buffer->tail = (uint8_t)((buffer->tail + 1u) % buffer->capacity);
    buffer->count--;
    return true;
}

bool stc_ring_buffer_peek(const stc_ring_buffer_t *buffer, uint8_t *value)
{
    if ((buffer == NULL) || (value == NULL) || stc_ring_buffer_is_empty(buffer)) {
        return false;
    }

    *value = buffer->data[buffer->tail];
    return true;
}

uint8_t stc_ring_buffer_size(const stc_ring_buffer_t *buffer)
{
    return (buffer == NULL) ? 0u : buffer->count;
}

uint8_t stc_ring_buffer_free(const stc_ring_buffer_t *buffer)
{
    if (buffer == NULL) {
        return 0u;
    }

    return (uint8_t)(buffer->capacity - buffer->count);
}

bool stc_ring_buffer_is_empty(const stc_ring_buffer_t *buffer)
{
    return (buffer == NULL) || (buffer->count == 0u);
}

bool stc_ring_buffer_is_full(const stc_ring_buffer_t *buffer)
{
    return (buffer != NULL) && (buffer->count >= buffer->capacity);
}
