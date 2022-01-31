/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include <jgui/debug.h>
#include <jgui/value.h>

typedef enum {
    JG_NONE,
    JG_BOOLEAN,
    JG_INTEGER,
    JG_DOUBLE,
    JG_STRING,
    JG_RAW,
    JG_OBJECT
} jg_value_type;

jg_value jg_none() {
    return (jg_value){
        ._type = JG_NONE
    };
}

jg_value jg_bool(bool value) {
    return (jg_value){
        ._type = JG_BOOLEAN,
        ._value._bool = value
    };
}

jg_value jg_int(int value) {
    return (jg_value){
        ._type = JG_INTEGER,
        ._value._int = value
    };
}

jg_value jg_double(double value) {
    return (jg_value){
        ._type = JG_DOUBLE,
        ._value._double = value
    };
}

jg_value jg_string(const char* value) {
    return (jg_value){
        ._type = JG_STRING,
        ._value._string = value
    };
}

jg_value jg_raw(void* value) {
    return (jg_value){
        ._type = JG_RAW,
        ._value._raw = value
    };
}

jg_value jg_object(void* value) {
    return (jg_value){
        ._type = JG_OBJECT,
        ._value._object = value
    };
}

bool jg_is_none(jg_value value) { return value._type == JG_NONE; }
bool jg_is_bool(jg_value value) { return value._type == JG_BOOLEAN; }
bool jg_is_int(jg_value value) { return value._type == JG_INTEGER; }
bool jg_is_double(jg_value value) { return value._type == JG_DOUBLE; }
bool jg_is_string(jg_value value) { return value._type == JG_STRING; }
bool jg_is_raw(jg_value value) { return value._type == JG_RAW; }
bool jg_is_object(jg_value value) { return value._type == JG_OBJECT; }

bool jg_to_bool(jg_value value) {
    assert(jg_is_bool(value));
    return value._value._bool;
}

int jg_to_int(jg_value value) {
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
