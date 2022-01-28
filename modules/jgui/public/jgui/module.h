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

typedef struct jg_module_definition_s jg_module_definition;
typedef struct jg_class_definition_s jg_class_definition;
typedef struct jg_module_s jg_module;
typedef struct jg_class_s jg_class;

/**
 * Create a new module definition.
 *
 * @return The newly created class module.
 */
jg_module_definition* jg_module_new();

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

jg_class_definition* jg_module_add_class(
    jg_module_definition* module,
    const char* id,
    const char* parent_id,
    size_t size,
    size_t align
);

/**
 * Create a usable jg_module from a jg_module_definition
 * 
 * @param definition 
 * @return jg_module* 
 */
jg_module* jg_module_build(jg_module_definition* module);

/**
 * Destroy a jg_module, and all it's registered classes.
 *
 * @param module The module to destroy.
 */
void jg_module_free(jg_module* module);

/**
 * Get a class using it's definition as a key.
 *
 * If the class wasn't registered, the function will return NULL.
 *
 * @param module   A previously created jg_module.
 * @param definition A class definition callback.
 *
 * @return The newly created class.
 */

const jg_class* jg_module_get_class(const jg_module* module, const char* id);

#endif