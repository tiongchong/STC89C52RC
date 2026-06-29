#ifndef TEST_REGISTRY_H
#define TEST_REGISTRY_H

#include <stdint.h>

/**
 * Test function signature
 * Returns 0 for PASS, -1 for FAIL
 * May print diagnostic info via cli_printf()
 */
typedef int (*stc89c52rc_test_fn_t)(int argc, char *argv[]);

/**
 * Test case descriptor
 */
typedef struct {
    const char *name;           /**< Hierarchical test name, e.g., "gpio.write" */
    const char *help;           /**< Brief help text */
    stc89c52rc_test_fn_t run;   /**< Test function pointer */
} stc89c52rc_test_case_t;

/**
 * Get the test registry
 * @param count Output: number of tests in registry
 * @return Pointer to array of test cases
 */
const stc89c52rc_test_case_t *stc89c52rc_test_registry(size_t *count);

/**
 * Find a test by name (case-sensitive)
 * @param name Test name, e.g., "gpio.write"
 * @return Pointer to test case, or NULL if not found
 */
const stc89c52rc_test_case_t *stc89c52rc_test_find(const char *name);

#endif // TEST_REGISTRY_H
