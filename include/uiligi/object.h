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
#pragma once

#include <stddef.h>

#include <uiligi/value.h>

typedef struct _UlgModule UlgModule;
typedef struct _UlgClass UlgClass;
typedef struct _UlgProperty UlgProperty;
typedef struct _UlgModuleDefinition UlgModuleDefinition;
typedef struct _UlgClassDefinition UlgClassDefinition;

/**
 * Retrieve a class by it's type name.
 *
 * @param module A previously created and initialized UlgModule.
 * @param name    The class name to get.
 *
 * @return The matching class, or NULL if it was not found in the module.
 */

const UlgClass* ulg_class_get_by_name(UlgModule* module, const char* name);

/**
 * void virtual table structure. See ulg_class_edit_vtable to implement methods on
 * class inheriting from void.
 */
typedef struct _UlgObjectVT {
    /** Called after the given object is allocated and initialized. */
    void (*init)(void*);

    /** Called after the given object is allocated and initialized. */
    void (*cleanup)(void*);
} UlgObjectVT;

/**
 * @brief void class definition.
 */
void ulg_object_type(UlgClassDefinition* definition);

/**
 * Create an object of the given class.
 */
[[nodiscard]] void* ulg_object_new(const UlgClass* class_);

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
UlgValue ulg_object_get(const void* object, const char* property_name);

/** Equivalent to ulg_to_bool(ulg_object_get(object, property_name)) */
bool ulg_object_get_bool(const void* object, const char* property_name);

/** Equivalent to ulg_to_int(ulg_object_get(object, property_name)) */
int ulg_object_get_int(const void* object, const char* property_name);

/** Equivalent to ulg_to_float(ulg_object_get(object, property_name)) */
float ulg_object_get_float(const void* object, const char* property_name);

/** Equivalent to ulg_to_string(ulg_object_get(object, property_name)) */
const char* ulg_object_get_string(const void* object, const char* property_name);

/** Equivalent to ulg_to_raw(ulg_object_get(object, property_name)) */
const void* ulg_object_get_raw(const void* object, const char* property_name);

/** Equivalent to ulg_to_object(ulg_object_get(object, property_name)) */
const void* ulg_object_get_object(const void* object, const char* property_name);

/**
 * Set a property on an object.
 *
 * @param object        The target object on which to set the property.
 * @param property_name Name of the property to set.
 * @param value         Value to set the property to.
 */
void ulg_object_set(void* object, const char* property_name, UlgValue value);
