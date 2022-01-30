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
#ifndef JGUI_PRIVATE_MODULE_H
#define JGUI_PRIVATE_MODULE_H

#include "common/index.h"

typedef struct jg_allocator_s jg_allocator;
typedef struct jg_class_definition_s jg_class_definition;
typedef struct jg_class_s jg_class;
typedef struct jg_module_definition_s jg_module_definition;
typedef struct jg_module_s jg_module;
typedef struct jg_pool_s jg_pool;

struct jg_module_definition_s {
    jg_allocator* allocator;
    const char* namespace;
    jg_class_definition* first_class;
    jg_module_definition* next_module;
};

struct jg_module_s {
    jg_index class_index;
    jg_class* class_array;
};

jg_index jg_module_build_index(jg_module_definition* first_module_definition, jg_pool* pools);

jg_class* jg_module_get_class(jg_module* module, const char* id);

#endif
