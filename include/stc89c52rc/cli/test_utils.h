#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Parse test arguments in key=value format
 * Example: test_arg_u32(argc, argv, "pin", 5) retrieves "pin=<value>" or returns 5
 */
uint32_t test_arg_u32(int argc, char **argv, const char *key, uint32_t default_value);

/**
 * Parse a boolean test argument
 * Recognizes: "1", "true", "yes", "on" as true
 */
bool test_arg_bool(int argc, char **argv, const char *key, bool default_value);

/**
 * Get string value of a test argument (returns NULL if not found)
 */
const char *test_arg_value(int argc, char **argv, const char *key);

/**
 * Return value for PASS (0)
 */
inline int test_pass(void) { return 0; }

/**
 * Return value for FAIL (-1)
 */
inline int test_fail(void) { return -1; }

#endif // TEST_UTILS_H
