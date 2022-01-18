/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <assert.h>

#include "uiligi/value.h"

UlgValue ulg_bool(bool value) {
    return (UlgValue){
        ._type = _ULG_BOOLEAN,
        ._value._bool = value
    };
}

UlgValue ulg_int(int value) {
    return (UlgValue){
        ._type = _ULG_INTEGER,
        ._value._int = value
    };
}

UlgValue ulg_double(double value) {
    return (UlgValue){
        ._type = _ULG_DOUBLE,
        ._value._double = value
    };
}

UlgValue ulg_string(const char* value) {
    return (UlgValue){
        ._type = _ULG_STRING,
        ._value._string = value
    };
}

UlgValue ulg_raw(void* value) {
    return (UlgValue){
        ._type = _ULG_RAW,
        ._value._raw = value
    };
}

UlgValue ulg_object(void* value) {
    return (UlgValue){
        ._type = _ULG_OBJECT,
        ._value._object = value
    };
}

bool ulg_is_bool(UlgValue value) { return value._type == _ULG_BOOLEAN; }
bool ulg_is_int(UlgValue value) { return value._type == _ULG_INTEGER; }
bool ulg_is_double(UlgValue value) { return value._type == _ULG_DOUBLE; }
bool ulg_is_string(UlgValue value) { return value._type == _ULG_STRING; }
bool ulg_is_raw(UlgValue value) { return value._type == _ULG_RAW; }
bool ulg_is_object(UlgValue value) { return value._type == _ULG_OBJECT; }

bool ulg_to_bool(UlgValue value) {
    assert(ulg_is_bool(value));
    return value._value._bool;
}

long ulg_to_int(UlgValue value) {
    assert(ulg_is_int(value));
    return value._value._int;
}

double ulg_to_double(UlgValue value) {
    assert(ulg_is_double(value));
    return value._value._double;
}

const char* ulg_to_string(UlgValue value) {
    assert(ulg_is_string(value));
    return value._value._string;
}

const void* ulg_to_raw(UlgValue value) {
    assert(ulg_is_raw(value));
    return value._value._raw;
}

void* ulg_to_object(UlgValue value) { 
    assert(ulg_is_object(value));
    return value._value._object;
}
