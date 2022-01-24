/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <assert.h>

#include "uiligi/value.h"

typedef enum {
    ULG_BOOLEAN,
    ULG_INTEGER,
    ULG_DOUBLE,
    ULG_STRING,
    ULG_RAW,
    ULG_OBJECT
} ulg_value_type;

ulg_value_t ulg_bool(bool value) {
    return (ulg_value_t){
        ._type = ULG_BOOLEAN,
        ._value._bool = value
    };
}

ulg_value_t ulg_int(int value) {
    return (ulg_value_t){
        ._type = ULG_INTEGER,
        ._value._int = value
    };
}

ulg_value_t ulg_double(double value) {
    return (ulg_value_t){
        ._type = ULG_DOUBLE,
        ._value._double = value
    };
}

ulg_value_t ulg_string(const char* value) {
    return (ulg_value_t){
        ._type = ULG_STRING,
        ._value._string = value
    };
}

ulg_value_t ulg_raw(void* value) {
    return (ulg_value_t){
        ._type = ULG_RAW,
        ._value._raw = value
    };
}

ulg_value_t ulg_object(void* value) {
    return (ulg_value_t){
        ._type = ULG_OBJECT,
        ._value._object = value
    };
}

bool ulg_is_bool(ulg_value_t value) { return value._type == ULG_BOOLEAN; }
bool ulg_is_int(ulg_value_t value) { return value._type == ULG_INTEGER; }
bool ulg_is_double(ulg_value_t value) { return value._type == ULG_DOUBLE; }
bool ulg_is_string(ulg_value_t value) { return value._type == ULG_STRING; }
bool ulg_is_raw(ulg_value_t value) { return value._type == ULG_RAW; }
bool ulg_is_object(ulg_value_t value) { return value._type == ULG_OBJECT; }

bool ulg_to_bool(ulg_value_t value) {
    assert(ulg_is_bool(value));
    return value._value._bool;
}

long ulg_to_int(ulg_value_t value) {
    assert(ulg_is_int(value));
    return value._value._int;
}

double ulg_to_double(ulg_value_t value) {
    assert(ulg_is_double(value));
    return value._value._double;
}

const char* ulg_to_string(ulg_value_t value) {
    assert(ulg_is_string(value));
    return value._value._string;
}

const void* ulg_to_raw(ulg_value_t value) {
    assert(ulg_is_raw(value));
    return value._value._raw;
}

void* ulg_to_object(ulg_value_t value) { 
    assert(ulg_is_object(value));
    return value._value._object;
}
