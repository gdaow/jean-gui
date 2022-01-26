/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * Implementation of the void model.
 * 
 * TODO: Finish documentation here.
 *
 */
#ifndef INCLUDE_MEZO_OBJECT_H
#define INCLUDE_MEZO_OBJECT_H

#include <stddef.h>

#include <mezo/value.h>

static const size_t MAX_NAME_LENGTH = 256;

typedef struct mz_module_s mz_module;
typedef struct mz_class_s mz_class;
typedef struct mz_member_s mz_member;
typedef struct mz_module_definition_s mz_module_definition;
typedef struct mz_class_definition_s mz_class_definition;

/**
 * Create a new module definition.
 *
 * @return The newly created class module.
 */
mz_module_definition* mz_module_new();

/**
 * Register a class in the class module, using the given definition.
 *
 * It will assert if the class is already registered in the module.
 *
 * @param module    A previously created mz_module.
 * @param definition A class definition callback.
 *
 * @return The newly created class.
 */

mz_class_definition* mz_class_new(
    mz_module_definition* module,
    const char* name,
    const char* parent,
    size_t size,
    size_t align
);

/** Callback for voids property getter.*/
typedef mz_value (*mz_getter_t)(const void*);

/** Callback for voids property setter.*/
typedef void (*mz_setter_t)(void*, const mz_value);

/**
 * Add a property to this class.
 * @param class_  Add a property to this class.
 * @param name    The name of the property.
 * @param setter  Callback that get the property value.
 * @param getter  Callback that set the property value.
 */
void mz_class_add_property(
    mz_class_definition* class_,
    const char* name,
    mz_getter_t getter,
    mz_setter_t setter
);

/**
 * Create a usable mz_module from a mz_module_definition
 * 
 * @param definition 
 * @return mz_module* 
 */
mz_module* mz_module_build(mz_module_definition* module);

/**
 * Destroy a mz_module, and all it's registered classes.
 *
 * @param module The module to destroy.
 */
void mz_module_free(mz_module* module);

/**
 * Get a class using it's definition as a key.
 *
 * If the class wasn't registered, the function will return NULL.
 *
 * @param module   A previously created mz_module.
 * @param definition A class definition callback.
 *
 * @return The newly created class.
 */

const mz_class* mz_class_get(const mz_module* module, const char* name);

#define mz_OBJECT "Object"

/**
 * Create an object of the given class.
 */
void* mz_object_new(const mz_class* class_);

/**
 * Release the given void.
 */
void mz_object_free(void* object);

/**
 * Get a property from an object.
 *
 * @param object        The target object on which to set the property.
 * @param property_name Name of the property to set.
 * @return              The property value.
 */
mz_value mz_object_get(const void* object, const char* property_name);

/**
 * Set a property on an object.
 *
 * @param object        The target object on which to set the property.
 * @param property_name Name of the property to set.
 * @param value         Value to set the property to.
 */
void mz_object_set(void* object, const char* property_name, mz_value value);

#endif
