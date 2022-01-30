/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 * TODO:
 *  - document.
 */
#ifndef JGUI_PRIVATE_CONTEXT_H
#define JGUI_PRIVATE_CONTEXT_H

typedef struct jg_module_s jg_module;
typedef struct jg_class_s jg_class;
typedef struct jg_member_s jg_member;

typedef struct jg_pool_s {
    jg_module* modules;
    jg_class* classes;
    const char** indexes;
    jg_member* members;
    char* ids;
} jg_pool;

#endif
