/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * Generic value wrapper, used to get / set properties.
 *
 * Why it only contains int and double values, and no uint... ?
 * As uiligi main purpose is to bind a graphical ui to an underlying view model, and the combinatory
 * of needed conversion exploses when adding literal types, I made the choice of providing only support
 * for those two types, leaving to the programmer the burden of representing the model _value with only
 * those two types. This simplifies the UI interface, as well as value conversion during binding.
 *
 */
#pragma once
#include <stdbool.h>

typedef enum {
    _ULG_BOOLEAN,
    _ULG_INTEGER,
    _ULG_DOUBLE,
    _ULG_STRING,
    _ULG_RAW,
    _ULG_OBJECT
} _UlgValueType;

typedef struct {
    _UlgValueType _type;
    union {
        bool _bool;
        const char* _string;
	    int _int;
        double _double;
        void* _raw;
        void* _object;
    } _value;
} UlgValue;

UlgValue ulg_bool(bool value);
UlgValue ulg_int(int value);
UlgValue ulg_double(double value);
UlgValue ulg_string(const char* value);
UlgValue ulg_raw(void* value);
UlgValue ulg_object(void* value);

bool ulg_is_bool(UlgValue value);
bool ulg_is_int(UlgValue value);
bool ulg_is_double(UlgValue value);
bool ulg_is_string(UlgValue value);
bool ulg_is_raw(UlgValue value);
bool ulg_is_object(UlgValue value);

bool ulg_to_bool(UlgValue value);
long ulg_to_int(UlgValue value);
double ulg_to_double(UlgValue value);
const char* ulg_to_string(UlgValue value);
const void* ulg_to_raw(UlgValue value);
void* ulg_to_object(UlgValue value);
