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
#ifndef INCLUDE_JEAN_GUI_OBJECT_H
#define INCLUDE_JEAN_GUI_OBJECT_H

#include <stddef.h>

#include <jg/value.h>

typedef struct jg_module_s jg_module;
typedef struct jg_class_s jg_class;
typedef struct jg_member_s jg_member;
typedef struct jg_module_definition_s jg_module_definition;
typedef struct jg_class_definition_s jg_class_definition;
typedef struct jg_arguments_s jg_arguments;

typedef jg_value (*jg_method)(jg_value* arguments, size_t argument_count);

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

jg_class_definition* jg_class_new(
    jg_module_definition* module,
    const char* id,
    const char* parent,
    size_t size,
    size_t align
);

/** Callback for voids property getter.*/
typedef jg_value (*jg_getter_t)(const void*);

/** Callback for voids property setter.*/
typedef void (*jg_setter_t)(void*, const jg_value);

/**
 * Add a property to this class.
 * @param class_  Add a property to this class.
 * @param id    The id of the property.
 * @param setter  Callback that get the property value.
 * @param getter  Callback that set the property value.
 */
void jg_class_add_property(
    jg_class_definition* class_,
    const char* id,
    jg_getter_t getter,
    jg_setter_t setter
);

/**
 * Add or override a method for this class.
 * @param class_  Add a property to this class.
 * @param id    The id of the property.
 * @param setter  Callback that get the property value.
 * @param getter  Callback that set the property value.
 */
void jg_class_add_method(
    jg_class_definition* class_,
    const char* id,
    jg_method method
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

const jg_class* jg_class_get(const jg_module* module, const char* id);

#define jg_object_class_id "Object"

/**
 * Create an object of the given class.
 */
void* jg_object_new(const jg_class* class_);

/**
 * Release the given void.
 */
void jg_object_free(void* object);

/**
 * Get a property from an object.
 *
 * @param object        The target object on which to set the property.
 * @param property_id Name of the property to set.
 * @return              The property value.
 */
jg_value jg_object_get(const void* object, const char* property_id);

/**
 * Set a property on an object.
 *
 * @param object        The target object on which to set the property.
 * @param property_id Name of the property to set.
 * @param value         Value to set the property to.
 */
void jg_object_set(void* object, const char* property_id, jg_value value);

/**
 * Call a method on an object.
 *
 * @param object      The target object on which to set the property.
 * @param property_id Name of the property to set.
 * @return            The property value.
 */
jg_value jg_object_call(
    const void* object,
    const char* method_id,
    jg_value* arguments,
    size_t argument_count
);

#endif
