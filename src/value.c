/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <assert.h>

#include "jg/value.h"

typedef enum {
    jg_BOOLEAN,
    jg_INTEGER,
    jg_DOUBLE,
    jg_STRING,
    jg_RAW,
    jg_OBJECT
} jg_valueype;

jg_value jg_bool(bool value) {
    return (jg_value){
        ._type = jg_BOOLEAN,
        ._value._bool = value
    };
}

jg_value jg_int(int value) {
    return (jg_value){
        ._type = jg_INTEGER,
        ._value._int = value
    };
}

jg_value jg_double(double value) {
    return (jg_value){
        ._type = jg_DOUBLE,
        ._value._double = value
    };
}

jg_value jg_string(const char* value) {
    return (jg_value){
        ._type = jg_STRING,
        ._value._string = value
    };
}

jg_value jg_raw(void* value) {
    return (jg_value){
        ._type = jg_RAW,
        ._value._raw = value
    };
}

jg_value jg_object(void* value) {
    return (jg_value){
        ._type = jg_OBJECT,
        ._value._object = value
    };
}

bool jg_is_bool(jg_value value) { return value._type == jg_BOOLEAN; }
bool jg_is_int(jg_value value) { return value._type == jg_INTEGER; }
bool jg_is_double(jg_value value) { return value._type == jg_DOUBLE; }
bool jg_is_string(jg_value value) { return value._type == jg_STRING; }
bool jg_is_raw(jg_value value) { return value._type == jg_RAW; }
bool jg_is_object(jg_value value) { return value._type == jg_OBJECT; }

bool jg_to_bool(jg_value value) {
    assert(jg_is_bool(value));
    return value._value._bool;
}

long jg_to_int(jg_value value) {
    assert(jg_is_int(value));
    return value._value._int;
}

double jg_to_double(jg_value value) {
    assert(jg_is_double(value));
    return value._value._double;
}

const char* jg_to_string(jg_value value) {
    assert(jg_is_string(value));
    return value._value._string;
}

const void* jg_to_raw(jg_value value) {
    assert(jg_is_raw(value));
    return value._value._raw;
}

void* jg_to_object(jg_value value) { 
    assert(jg_is_object(value));
    return value._value._object;
}
