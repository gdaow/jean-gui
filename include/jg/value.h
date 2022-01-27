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
#ifndef INCLUDE_MEZO_VALUE_H_
#define INCLUDE_MEZO_VALUE_H_
#include <stdbool.h>

/* This struct member's are not meant to be used directly, and are not guaranteed to stay stable.
 * To handle jg_values, use the access function defined below. */
typedef struct jg_value_s {
    int _type;
    union {
        bool _bool;
        const char* _string;
	    int _int;
        double _double;
        void* _raw;
        void* _object;
    } _value;
} jg_value;

jg_value jg_bool(bool value);
jg_value jg_int(int value);
jg_value jg_double(double value);
jg_value jg_string(const char* value);
jg_value jg_raw(void* value);
jg_value jg_object(void* value);

bool jg_is_bool(jg_value value);
bool jg_is_int(jg_value value);
bool jg_is_double(jg_value value);
bool jg_is_string(jg_value value);
bool jg_is_raw(jg_value value);
bool jg_is_object(jg_value value);

bool jg_to_bool(jg_value value);
long jg_to_int(jg_value value);
double jg_to_double(jg_value value);
const char* jg_to_string(jg_value value);
const void* jg_to_raw(jg_value value);
void* jg_to_object(jg_value value);

#endif
