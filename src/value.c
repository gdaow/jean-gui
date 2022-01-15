#include <assert.h>

#include "uiligi/value.h"


UlgValue ulg_value_from_str(const char* value) {
    UlgValue result = {
        .type = ULG_STRING,
        .data = (void *)value
    };

    return result;
}

/**
 * Get a string form a generic value. Will assert if the given value is of the wrong type.
 * @param value The generic value to convert to string.
 * @return The string contained in the generic value.
 */
const char* ulg_value_to_str(UlgValue value) {
    assert(value.type == ULG_STRING);
    return (const char*)value.data;
}
