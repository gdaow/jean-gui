/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 * Helpers to handle arguments in dynamic jg_object calls. jg_arguments
 * is simply a list of jg_values, peeked in fifo order by the called
 * function.
 */
#ifndef JGUI_OBJECT_MODEL_ARGUMENTS_H
#define JGUI_OBJECT_MODEL_ARGUMENTS_H

#include <stdbool.h>
#include <stddef.h>

#include <jgui/misc/utils.h>

jg_forward_declare(jg_value)
jg_forward_declare(jg_arguments)

/**
 * Initialize an arguments structure.
 *
 * @param arguments An array of jg_value representing these arguments.
 * @param size      Size of the arguments array.
 *
 * @return Initialized arguments.
 */
jg_arguments jg_arguments_init(jg_value* arguments, size_t size);

/**
 * Helper macro to initialize arguments from a jg_value array.
 *
 * @param ... array of jg_value to init the arguments from.
 */
#define jg_arguments_new(...) jg_arguments_init((jg_value[]){__VA_ARGS__}, sizeof((jg_value[]){__VA_ARGS__}))

/**
 * Get the next jg_value from the given arguments.
 *
 * If the arguments contains no more value, the behavior is undefined.
 *
 * @param arguments Arguments to pop the value from.
 *
 * @return Next jg_value arguments contains.
 */
jg_value jg_argument_pop(jg_arguments* arguments);

/**
 * Get next argument as a bool value.
 *
 * If the next argument isn't a bool, or there is no more arguments, the behavior is undefined.
 *
 * @param arguments Arguments to pop the value from.
 */
bool jg_pop_bool(jg_arguments* arguments);

/**
 * Get next argument as a int value.
 *
 * If the next argument isn't a int, or there is no more arguments, the behavior is undefined.
 *
 * @param arguments Arguments to pop the value from.
 */
int jg_pop_int(jg_arguments* arguments);

/**
 * Get next argument as a double value.
 *
 * If the next argument isn't a double, or there is no more arguments, the behavior is undefined.
 *
 * @param arguments Arguments to pop the value from.
 */
double jg_pop_double(jg_arguments* arguments);

/**
 * Get next argument as a string value.
 *
 * If the next argument isn't a string, or there is no more arguments, the behavior is undefined.
 *
 * @param arguments Arguments to pop the value from.
 */
const char* jg_pop_string(jg_arguments* arguments);

/**
 * Get next argument as a raw pointer value.
 *
 * If the next argument isn't a raw pointer, or there is no more arguments, the behavior is undefined.
 *
 * @param arguments Arguments to pop the value from.
 */
const void* jg_pop_raw(jg_arguments* arguments);

/**
 * Get next argument as a object value.
 *
 * If the next argument isn't a object, or there is no more arguments, the behavior is undefined.
 *
 * @param arguments Arguments to pop the value from.
 */
void* jg_pop_object(jg_arguments* arguments);

/**
 * /!\ Private struct (see doc-private-struct) /!\
 */
typedef struct jg_arguments_s {
    jg_value* _start;
    jg_value* _end;
} jg_arguments;

#endif

