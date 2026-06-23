#include <stdio.h>
#include "minitest.h"

unsigned int mt_assertions = 0u;
unsigned int mt_failures = 0u;

void test_crc8_known_vector(void);
void test_crc8_incremental_matches_bulk(void);
void test_ring_buffer_push_pop_order(void);
void test_ring_buffer_wrap_and_full(void);
void test_ring_buffer_rejects_invalid_init(void);

int main(void)
{
    MT_RUN(test_crc8_known_vector);
    MT_RUN(test_crc8_incremental_matches_bulk);
    MT_RUN(test_ring_buffer_push_pop_order);
    MT_RUN(test_ring_buffer_wrap_and_full);
    MT_RUN(test_ring_buffer_rejects_invalid_init);

    printf("%u assertions, %u failures\n", mt_assertions, mt_failures);
    return (mt_failures == 0u) ? 0 : 1;
}
