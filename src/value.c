/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <assert.h>

#include "mezo/value.h"

typedef enum {
    mz_BOOLEAN,
    mz_INTEGER,
    mz_DOUBLE,
    mz_STRING,
    mz_RAW,
    mz_OBJECT
} mz_valueype;

mz_value mz_bool(bool value) {
    return (mz_value){
        ._type = mz_BOOLEAN,
        ._value._bool = value
    };
}

mz_value mz_int(int value) {
    return (mz_value){
        ._type = mz_INTEGER,
        ._value._int = value
    };
}

mz_value mz_double(double value) {
    return (mz_value){
        ._type = mz_DOUBLE,
        ._value._double = value
    };
}

mz_value mz_string(const char* value) {
    return (mz_value){
        ._type = mz_STRING,
        ._value._string = value
    };
}

mz_value mz_raw(void* value) {
    return (mz_value){
        ._type = mz_RAW,
        ._value._raw = value
    };
}

mz_value mz_object(void* value) {
    return (mz_value){
        ._type = mz_OBJECT,
        ._value._object = value
    };
}

bool mz_is_bool(mz_value value) { return value._type == mz_BOOLEAN; }
bool mz_is_int(mz_value value) { return value._type == mz_INTEGER; }
bool mz_is_double(mz_value value) { return value._type == mz_DOUBLE; }
bool mz_is_string(mz_value value) { return value._type == mz_STRING; }
bool mz_is_raw(mz_value value) { return value._type == mz_RAW; }
bool mz_is_object(mz_value value) { return value._type == mz_OBJECT; }

bool mz_to_bool(mz_value value) {
    assert(mz_is_bool(value));
    return value._value._bool;
}

long mz_to_int(mz_value value) {
    assert(mz_is_int(value));
    return value._value._int;
}

double mz_to_double(mz_value value) {
    assert(mz_is_double(value));
    return value._value._double;
}

const char* mz_to_string(mz_value value) {
    assert(mz_is_string(value));
    return value._value._string;
}

const void* mz_to_raw(mz_value value) {
    assert(mz_is_raw(value));
    return value._value._raw;
}

void* mz_to_object(mz_value value) { 
    assert(mz_is_object(value));
    return value._value._object;
}
