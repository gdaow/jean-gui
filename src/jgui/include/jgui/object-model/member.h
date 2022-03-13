/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * Members encapsulate function pointers and can be registered into
 * jg_class for dynamic method calling and property getting / setting.
 *
 */
#ifndef JGUI_OBJECT_MODEL_MEMBER_H
#define JGUI_OBJECT_MODEL_MEMBER_H

#include <jgui/misc/utils.h>

#include <jgui/object-model/value.h>

jg_forward_declare(jg_value);
jg_forward_declare(jg_arguments);

/**
 * Pointer to a dynamic method that can be registered in a jg_class.
 */
typedef jg_value (*jg_method)(jg_arguments*);

/**
 * Pointer to a property getter that can be registered in a jg_class.
 */
typedef jg_value (*jg_getter)(const void*);

/**
 * Pointer to a property setter that can be registered in a jg_class.
 */
typedef void (*jg_setter)(void*, const jg_value);

#endif

