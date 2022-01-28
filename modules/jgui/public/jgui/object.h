/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * TODO: document.
 *
 */
#ifndef JGUI_PUBLIC_JGUI_OBJECT_H
#define JGUI_PUBLIC_JGUI_OBJECT_H

#include <stddef.h>

#include <jgui/value.h>

typedef struct jg_module_s jg_module;
typedef struct jg_class_s jg_class;
typedef struct jg_member_s jg_member;
typedef struct jg_module_definition_s jg_module_definition;
typedef struct jg_class_definition_s jg_class_definition;

typedef jg_value (*jg_method)(jg_value* arguments);


extern const char* jg_object_class_id;

/**
 * Create a new object.
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
jg_value jg_object_call(const void* object, const char* method_id, jg_value* arguments);

#endif
