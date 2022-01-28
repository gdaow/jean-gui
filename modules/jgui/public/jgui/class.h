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

typedef struct jg_class_definition_s jg_class_definition;
typedef struct jg_class_s jg_class;

typedef jg_value (*jg_method)(jg_value* arguments);

/** Callback for voids property getter.*/
typedef jg_value (*jg_getter_t)(const void*);

/** Callback for voids property setter.*/
typedef void (*jg_setter_t)(void*, const jg_value);


void jg_class_add_method(
    jg_class_definition* class_,
    const char* id,
    jg_method method
);

void jg_class_add_property(
    jg_class_definition* class_,
    const char* id,
    jg_getter_t getter,
    jg_setter_t setter
);

#endif
