/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * TODO:
 *  - document.
 *
 */
#ifndef JGUI_PUBLIC_JGUI_CLASS_H
#define JGUI_PUBLIC_JGUI_CLASS_H

#include <jgui/value.h>

typedef struct jg_class_builder_s jg_class_builder;
typedef struct jg_class_s jg_class;
typedef struct jg_arguments_s jg_arguments;


typedef void (*jg_constructor)(void* object);
typedef void (*jg_destructor)(void* object);

typedef jg_value (*jg_method)(jg_arguments*);

/** Callback for voids property getter.*/
typedef jg_value (*jg_getter_t)(const void*);

/** Callback for voids property setter.*/
typedef void (*jg_setter_t)(void*, const jg_value);

void jg_class_set_constructor(jg_class_builder* class_builder, jg_constructor constructor, jg_destructor destructor);

void jg_class_add_method(jg_class_builder* class_builder, const char* id, jg_method method);

void jg_class_add_property(
    jg_class_builder* class_builder,
    const char* id,
    jg_getter_t getter,
    jg_setter_t setter
);

bool jg_pop_bool(jg_arguments* arguments);
int jg_pop_int(jg_arguments* arguments);
double jg_pop_double(jg_arguments* arguments);
const char* jg_pop_string(jg_arguments* arguments);
const void* jg_pop_raw(jg_arguments* arguments);
void* jg_pop_object(jg_arguments* arguments);
bool jg_arguments_error(jg_arguments* arguments);

#endif
