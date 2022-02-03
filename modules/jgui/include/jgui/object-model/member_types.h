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
#ifndef JGUI_PUBLIC_JGUI_MEMBER_TYPES_H
#define JGUI_PUBLIC_JGUI_MEMBER_TYPES_H

#include <jgui/misc/utils.h>

jg_forward_declare(jg_value)
jg_forward_declare(jg_arguments)

typedef struct jg_arguments_s jg_arguments;

typedef jg_value (*jg_method)(jg_arguments*);

/** Callback for voids property getter.*/
typedef jg_value (*jg_getter)(const void*);

/** Callback for voids property setter.*/
typedef void (*jg_setter)(void*, const jg_value);

#endif

