#include "stc89c52rc/cli/test_utils.h"
#include <string.h>

/**
 * Find the value part of a key=value argument
 */
static const char *find_arg_value(int argc, char **argv, const char *key)
{
    if (!key || !argv) return NULL;
    
    size_t key_len = strlen(key);
    
    for (int i = 0; i < argc; i++) {
        if (argv[i] == NULL) continue;
        
        // Check if arg starts with "key="
        if (strncmp(argv[i], key, key_len) == 0 && argv[i][key_len] == '=') {
            return &argv[i][key_len + 1];
        }
    }
    
    return NULL;
}

static uint8_t parse_digit(char value, uint8_t *digit)
{
    if ((value >= '0') && (value <= '9')) {
        *digit = (uint8_t)(value - '0');
        return 1u;
    }
    if ((value >= 'a') && (value <= 'f')) {
        *digit = (uint8_t)(value - 'a' + 10);
        return 1u;
    }
    if ((value >= 'A') && (value <= 'F')) {
        *digit = (uint8_t)(value - 'A' + 10);
        return 1u;
    }

    return 0u;
}

static uint8_t parse_u32(const char *text, uint32_t *value)
{
    uint8_t base = 10u;
    uint8_t digit = 0u;
    uint8_t saw_digit = 0u;
    uint32_t result = 0UL;

    if ((text == 0) || (value == 0)) {
        return 0u;
    }

    if ((text[0] == '0') && ((text[1] == 'x') || (text[1] == 'X'))) {
        base = 16u;
        text += 2;
    }

    while ((*text != '\0') && (*text != ' ')) {
        if (!parse_digit(*text, &digit) || (digit >= base)) {
            return 0u;
        }

        result = (uint32_t)((result * base) + digit);
        saw_digit = 1u;
        text++;
    }

    while (*text == ' ') {
        text++;
    }

    if ((*text != '\0') || !saw_digit) {
        return 0u;
    }

    *value = result;
    return 1u;
}

uint32_t test_arg_u32(int argc, char **argv, const char *key, uint32_t default_value)
{
    const char *val = find_arg_value(argc, argv, key);
    uint32_t result;

    if (!val) return default_value;
    
    if (!parse_u32(val, &result)) {
        return default_value;
    }
    
    return result;
}

uint8_t test_arg_bool(int argc, char **argv, const char *key, uint8_t default_value)
{
    const char *val = find_arg_value(argc, argv, key);
    if (!val) return default_value;
    
    // Check for true values
    if (strcmp(val, "1") == 0 || 
        strcmp(val, "true") == 0 ||
        strcmp(val, "yes") == 0 ||
        strcmp(val, "on") == 0) {
        return 1;
    }
    
    // Check for false values
    if (strcmp(val, "0") == 0 || 
        strcmp(val, "false") == 0 ||
        strcmp(val, "no") == 0 ||
        strcmp(val, "off") == 0) {
        return 0;
    }
    
    return default_value;
}

const char *test_arg_value(int argc, char **argv, const char *key)
{
    return find_arg_value(argc, argv, key);
}
