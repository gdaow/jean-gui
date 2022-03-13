/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 * Modules are a group of classes that are associated with a namespace in
 * the context. These are used by templating to avoid name collisions when
 * instancing classes.
 *
 */
#ifndef JGUI_PRIVATE_OBJECT_MODEL_MODULE_H
#define JGUI_PRIVATE_OBJECT_MODEL_MODULE_H

#include <stddef.h>

#include "jgui/private/containers/index.h"

#include <jgui/misc/utils.h>

struct jg_module_s;

jg_forward_declare(jg_class);
jg_forward_declare(jg_module);

/**
 * Initialize a module.
 */
void jg_module_init(jg_module* module);

/**
 * Release memory used by this module instance.
 */
void jg_module_cleanup(jg_module* module);

/**
 * Register a class in this module.
 *
 * @param name   Name of the class to create.
 * @param parent Parent class to assign to the new class. See jg_class_init.
 * @param size   Size of objects wrapped by the new class. See jg_class_init.
 *
 * @return A pointer to a newly created class. You can register methods and
 *         properties in it using jg_class_add_property and
 *         jg_class_add_method.
 */
jg_class* jg_module_add_class(jg_module* module,
                              const char* name,
                              const jg_class* parent,
                              size_t size);

/**
 * Retrieve a class that was previously registered in the module using
 * jg_module_add_class.
 *
 * @param name   Name of the class to retrieve.
 *
 * @return A pointer to the class that was registered, or NULL if the class
 *         was not declared in this module.
 */
const jg_class* jg_module_get_class(const jg_module* module, const char* name);

struct jg_module_s {
	jg_index class_index;
};

#endif
