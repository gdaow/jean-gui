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

static const size_t MAX_NAME_LENGTH = 256;

typedef struct _UlgModule UlgModule;
typedef struct _UlgClass UlgClass;
typedef struct _UlgMember UlgMember;
typedef struct _UlgModuleDefinition UlgModuleDefinition;
typedef struct _UlgClassDefinition UlgClassDefinition;

/**
 * Create a new module definition.
 *
 * @return The newly created class module.
 */
UlgModuleDefinition* ulg_module_new();

/**
 * Register a class in the class module, using the given definition.
 *
 * It will assert if the class is already registered in the module.
 *
 * @param module    A previously created UlgModule.
 * @param definition A class definition callback.
 *
 * @return The newly created class.
 */

UlgClassDefinition* ulg_class_new(
    UlgModuleDefinition* module,
    const char* name,
    const char* parent,
    size_t size,
    size_t align
);

/** Callback for voids property getter.*/
typedef UlgValue (*UlgGetter)(const void*);

/** Callback for voids property setter.*/
typedef void (*UlgSetter)(void*, const UlgValue);

/**
 * Add a property to this class.
 * @param class_  Add a property to this class.
 * @param name    The name of the property.
 * @param setter  Callback that get the property value.
 * @param getter  Callback that set the property value.
 */
void ulg_class_add_property(
    UlgClassDefinition* definition,
    const char* name,
    UlgGetter getter,
    UlgSetter setter
);

/**
 * Create a usable UlgModule from a UlgModuleDefinition
 * 
 * @param definition 
 * @return UlgModule* 
 */
UlgModule* ulg_module_build(UlgModuleDefinition* module);

/**
 * Destroy a UlgModule, and all it's registered classes.
 *
 * @param module The module to destroy.
 */
void ulg_module_free(UlgModule* module);

/**
 * Get a class using it's definition as a key.
 *
 * If the class wasn't registered, the function will return NULL.
 *
 * @param module   A previously created UlgModule.
 * @param definition A class definition callback.
 *
 * @return The newly created class.
 */

const UlgClass* ulg_class_get(const UlgModule* module, const char* name);

#define ULG_OBJECT "Object"

/**
 * Create an object of the given class.
 */
void* ulg_object_new(const UlgClass* class_);

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
