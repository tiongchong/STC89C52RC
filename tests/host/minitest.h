#ifndef STC89C52RC_TESTS_MINITEST_H
#define STC89C52RC_TESTS_MINITEST_H

#include <stdio.h>

extern unsigned int mt_assertions;
extern unsigned int mt_failures;

#define MT_ASSERT(expr) do { \
    mt_assertions++; \
    if (!(expr)) { \
        mt_failures++; \
        printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #expr); \
        return; \
    } \
} while (0)

#define MT_RUN(test_fn) do { \
    unsigned int failures_before = mt_failures; \
    printf("RUN  %s\n", #test_fn); \
    test_fn(); \
    if (failures_before == mt_failures) { \
        printf("OK   %s\n", #test_fn); \
    } \
} while (0)

#endif
