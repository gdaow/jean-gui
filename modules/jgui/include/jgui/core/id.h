/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * TODO:
 *  - document.
 *  - Use functions instead of macros here.
 *
 */
#ifndef JGUI_INCLUDE_JGUI_CORE_ID_H
#define JGUI_INCLUDE_JGUI_CORE_ID_H
#include <stdbool.h>

typedef struct jg_id_s {
    const char* namespace;
    const char* name;
} jg_id;

#define jg_id_new(in_namespace, in_name) (\
    (jg_id) {\
        .namespace = (in_namespace),\
        .name = (in_name)\
    }\
)

bool jg_id_equal(jg_id a, jg_id b);

#define jg_empty_id(id) ((id).namespace == NULL && (id).name == NULL)

#define jg_valid_id(id) (\
    jg_empty_id(id) || (\
        (id).namespace != NULL &&\
        strlen((id).namespace) > 0 &&\
        (id).name != NULL &&\
        strlen((id).name) > 0\
    )\
)

#endif

