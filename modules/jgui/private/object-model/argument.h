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
#ifndef JGUI_PRIVATE_OBJECT_MODEL_ARGUMENT_H
#define JGUI_PRIVATE_OBJECT_MODEL_ARGUMENT_H

#include <jgui/misc/utils.h>

jg_forward_declare(jg_value)

typedef struct jg_arguments_s {
    jg_value* start;
    jg_value* end;
} jg_arguments;

int jg_pop_int(jg_arguments* arguments);

#endif

