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
#ifndef INCLUDE_UILIGI_OBJECT_H
#define INCLUDE_UILIGI_OBJECT_H

#include <stddef.h>

#include <uiligi/value.h>

static const size_t MAX_NAME_LENGTH = 256;

typedef struct ulg_module_s ulg_module_t;
typedef struct ulg_class_s ulg_class_t;
typedef struct ulg_member_s ulg_member_t;
typedef struct ulg_module_definition_s ulg_module_definition_t;
typedef struct ulg_class_definition_s ulg_class_definition_t;

/**
 * Create a new module definition.
 *
 * @return The newly created class module.
 */
ulg_module_definition_t* ulg_module_new();

/**
 * Register a class in the class module, using the given definition.
 *
 * It will assert if the class is already registered in the module.
 *
 * @param module    A previously created ulg_module_t.
 * @param definition A class definition callback.
 *
 * @return The newly created class.
 */

ulg_class_definition_t* ulg_class_new(
    ulg_module_definition_t* module,
    const char* name,
    const char* parent,
    size_t size,
    size_t align
);

/** Callback for voids property getter.*/
typedef ulg_value_t (*ulg_getter_t)(const void*);

/** Callback for voids property setter.*/
typedef void (*ulg_setter_t)(void*, const ulg_value_t);

/**
 * Add a property to this class.
 * @param class_  Add a property to this class.
 * @param name    The name of the property.
 * @param setter  Callback that get the property value.
 * @param getter  Callback that set the property value.
 */
void ulg_class_add_property(
    ulg_class_definition_t* class_,
    const char* name,
    ulg_getter_t getter,
    ulg_setter_t setter
);

/**
 * Create a usable ulg_module_t from a ulg_module_definition_t
 * 
 * @param definition 
 * @return ulg_module_t* 
 */
ulg_module_t* ulg_module_build(ulg_module_definition_t* module);

/**
 * Destroy a ulg_module_t, and all it's registered classes.
 *
 * @param module The module to destroy.
 */
void ulg_module_free(ulg_module_t* module);

/**
 * Get a class using it's definition as a key.
 *
 * If the class wasn't registered, the function will return NULL.
 *
 * @param module   A previously created ulg_module_t.
 * @param definition A class definition callback.
 *
 * @return The newly created class.
 */

const ulg_class_t* ulg_class_get(const ulg_module_t* module, const char* name);

#define ULG_OBJECT "Object"

/**
 * Create an object of the given class.
 */
void* ulg_object_new(const ulg_class_t* class_);

/**
 * Release the given void.
 */
void ulg_object_free(void* object);

/**
 * Get a property from an object.
 *
 * @param object        The target object on which to set the property.
 * @param property_name Name of the property to set.
 * @return              The property value.
 */
ulg_value_t ulg_object_get(const void* object, const char* property_name);

/**
 * Set a property on an object.
 *
 * @param object        The target object on which to set the property.
 * @param property_name Name of the property to set.
 * @param value         Value to set the property to.
 */
void ulg_object_set(void* object, const char* property_name, ulg_value_t value);

#endif
