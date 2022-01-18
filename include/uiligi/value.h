/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * Generic literal values wrapper.
 * TODO: Detail.
 *
 * ui-ligi value wrapper only support int and double, and no ulong, float... because the combinatorial
 * of default conversion between supported types rapidly exploses, and with it the complexity.
 * TODO: detail this and the choice I made.
 *
 */
#pragma once
#include <stdbool.h>

/* This struct member's are not meant to be used directly, and are not guaranteed to stay stable.
 * To handle UlgValues, use the access function defined below. */
typedef struct {
    int _type;
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
