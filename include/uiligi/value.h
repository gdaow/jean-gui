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
#ifndef INCLUDE_UILIGI_VALUE_H_
#define INCLUDE_UILIGI_VALUE_H_
#include <stdbool.h>

/* This struct member's are not meant to be used directly, and are not guaranteed to stay stable.
 * To handle ulg_value_ts, use the access function defined below. */
typedef struct ulg_value_s {
    int _type;
    union {
        bool _bool;
        const char* _string;
	    int _int;
        double _double;
        void* _raw;
        void* _object;
    } _value;
} ulg_value_t;

ulg_value_t ulg_bool(bool value);
ulg_value_t ulg_int(int value);
ulg_value_t ulg_double(double value);
ulg_value_t ulg_string(const char* value);
ulg_value_t ulg_raw(void* value);
ulg_value_t ulg_object(void* value);

bool ulg_is_bool(ulg_value_t value);
bool ulg_is_int(ulg_value_t value);
bool ulg_is_double(ulg_value_t value);
bool ulg_is_string(ulg_value_t value);
bool ulg_is_raw(ulg_value_t value);
bool ulg_is_object(ulg_value_t value);

bool ulg_to_bool(ulg_value_t value);
long ulg_to_int(ulg_value_t value);
double ulg_to_double(ulg_value_t value);
const char* ulg_to_string(ulg_value_t value);
const void* ulg_to_raw(ulg_value_t value);
void* ulg_to_object(ulg_value_t value);

#endif
