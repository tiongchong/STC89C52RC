#include "stc89c52rc/cli/test_utils.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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

uint32_t test_arg_u32(int argc, char **argv, const char *key, uint32_t default_value)
{
    const char *val = find_arg_value(argc, argv, key);
    if (!val) return default_value;
    
    // Parse as decimal or hex (0x prefix)
    char *endptr;
    uint32_t result = (uint32_t)strtoul(val, &endptr, 0);
    
    // If parsing failed, return default
    if (*endptr != '\0' && *endptr != ' ') {
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
