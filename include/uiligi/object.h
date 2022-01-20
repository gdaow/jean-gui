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
typedef struct _UlgClassFactory UlgClassFactory;

/**
 * Create a new class module, usable to retrieve classes by name.
 *
 * @return The newly created class module.
 */
UlgModule* ulg_module_new();

/**
 * Destroy a UlgModule, and all it's registered classes.
 *
 * @param module The module to destroy.
 */
void ulg_module_free(UlgModule* module);

/** Callback defining a type. The adress of the callback will be used as type id. */
typedef void (UlgClassDefinition)(UlgClassFactory*);

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

void ulg_class_register(UlgModule* module, UlgClassDefinition definition);

/**
 * Create a new class object.
 *
 * This method is meant to be called in a UlgClassDefinition callback only.
 *
 * @param factory           Opaque type passed to type definitions functions by the type module.
 * @param name              Name of the new class.
 * @param data_size         Size of the class data.
 * @param parent_definition Parent class definition function.
 * @param vtable_size       Size of the class virtual table. If 0 is given, the vtable of the parent
 *                          will be copied.
 */
void ulg_class_declare(
    UlgClassFactory* factory,
    const char* name,
    UlgClassDefinition parent_definition
);

/**
 * Get a pointer to this class vtable.
 *
 * When creating a class, the vtable is initialized by copying the parent class one,
 * so you can set only function that you want to override on it, function left alone
 * will be set to the parent's implementation.
 *
 * @param class_ The class for which to get the vtable.
 */
void* ulg_class_create_vtable(UlgClassFactory* factory, size_t vtable_size, size_t vtable_alignment);

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
void ulg_class_add_property(UlgClassFactory* factory, const char* name, UlgGetter getter, UlgSetter setter);

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

const UlgClass* ulg_class_get(UlgModule* module, UlgClassDefinition definition);

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
void ulg_object_type(UlgClassFactory* factory);

/**
 * Create an object of the given class.
 */
[[nodiscard]] void* ulg_object_new(const UlgClass* class_);

/**
 * Release the given void.
 */
void ulg_object_free(void* object);

/**
 * @brief Return the virtual table of this object.
 * 
 * @param object Get the virtual table for this object.
 * 
 * @return A pointer to the object's virtual table.
 */
const void* ulg_object_vtable(const void* object);

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
