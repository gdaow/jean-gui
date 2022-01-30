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
#ifndef JGUI_PUBLIC_JGUI_MODULE_H
#define JGUI_PUBLIC_JGUI_MODULE_H

#include <stddef.h>

typedef struct jg_module_builder_s jg_module_builder;
typedef struct jg_class_builder_s jg_class_builder;

/**
 * Register a class in the class module, using the given definition.
 *
 * It will assert if the class is already registered in the module.
 *
 * @param module    A previously created jg_module.
 * @param definition A class definition callback.
 *
 * @return The newly created class.
 */
jg_class_builder* jg_module_add_class(
    jg_module_builder* module,
    const char* id,
    const char* parent_namespace,
    const char* parent_id,
    size_t size
);

#endif
