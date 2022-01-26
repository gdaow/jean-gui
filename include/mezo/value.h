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
 * To handle mz_values, use the access function defined below. */
typedef struct mz_value_s {
    int _type;
    union {
        bool _bool;
        const char* _string;
	    int _int;
        double _double;
        void* _raw;
        void* _object;
    } _value;
} mz_value;

mz_value mz_bool(bool value);
mz_value mz_int(int value);
mz_value mz_double(double value);
mz_value mz_string(const char* value);
mz_value mz_raw(void* value);
mz_value mz_object(void* value);

bool mz_is_bool(mz_value value);
bool mz_is_int(mz_value value);
bool mz_is_double(mz_value value);
bool mz_is_string(mz_value value);
bool mz_is_raw(mz_value value);
bool mz_is_object(mz_value value);

bool mz_to_bool(mz_value value);
long mz_to_int(mz_value value);
double mz_to_double(mz_value value);
const char* mz_to_string(mz_value value);
const void* mz_to_raw(mz_value value);
void* mz_to_object(mz_value value);

#endif
