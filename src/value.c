/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <assert.h>

#include "uiligi/value.h"

UlgValue ulg_value_from_str(const char* value) {
    UlgValue result = {
        .type = ULG_STRING,
        .data.str_value = (char *)value
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
    return (const char*)value.data.str_value;
}
