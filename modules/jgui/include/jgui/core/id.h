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
#ifndef JGUI_CORE_ID
#define JGUI_CORE_ID

#include <stdbool.h>

typedef struct jg_id_s {
    const char* namespace_;
    const char* name;
} jg_id;

extern const jg_id JG_ID_NONE;

jg_id JG_ID(const char* namespace_, const char* name);

bool jg_id_is_none(jg_id id);
bool jg_id_is_valid(jg_id id);

#endif

