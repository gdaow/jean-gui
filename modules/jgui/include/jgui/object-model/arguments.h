/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 * TODO:
 *  -document
 */
#ifndef JGUI_OBJECT_MODEL_ARGUMENTS_H
#define JGUI_OBJECT_MODEL_ARGUMENTS_H

#include <stdbool.h>
#include <stddef.h>

#include <jgui/misc/utils.h>

jg_forward_declare(jg_value)

typedef struct jg_arguments_s {
    jg_value* start;
    jg_value* end;
} jg_arguments;

jg_arguments jg_arguments_init(jg_value* arguments, size_t size);

#define jg_arguments_new(...) jg_arguments_init((jg_value[]){__VA_ARGS__}, sizeof((jg_value[]){__VA_ARGS__}))

jg_value jg_argument_pop(jg_arguments* arguments);

bool jg_pop_bool(jg_arguments* arguments);
int jg_pop_int(jg_arguments* arguments);
double jg_pop_double(jg_arguments* arguments);
const char* jg_pop_string(jg_arguments* arguments);
void* jg_pop_raw(jg_arguments* arguments);
void* jg_pop_object(jg_arguments* arguments);

#endif

