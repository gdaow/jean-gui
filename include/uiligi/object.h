#pragma once

#include "uiligi/value.h"

typedef struct UlgObject UlgObject;
typedef struct UlgClass UlgClass;

/**
 * Create an object of the given class.
 */
UlgObject* ulg_object_create(UlgClass* ulg_class);

/**
 * Release the given UlgObject.
 */
void ulg_object_destroy(UlgObject* object);

/**
 * Set a property on an object.
 *
 * @param object        The target object on which to set the property.
 * @param property_name Name of the property to set.
 * @param value         Value to set the property to.
 */
void ulg_object_set(UlgObject* object, const char* property_name, UlgValue value);

/**
 * Get a property from an object.
 *
 * @param object        The target object on which to set the property.
 * @param property_name Name of the property to set.
 * @return              The property value.
 */
UlgValue ulg_object_get(UlgObject* object, const char* property_name);
