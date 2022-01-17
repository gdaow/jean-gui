/**
 * @brief Contains function to declare UlgObject classes, instanciate objects & set properties.
 * @date 2022-01-17
 * 
 * TODO : Explanations on all this.
 *
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 */
#pragma once

#include <stddef.h>

#include <uiligi/value.h>

typedef struct _UlgContext UlgContext;
typedef struct _UlgClass UlgClass;
typedef struct _UlgClassFactory UlgClassFactory;
typedef struct _UlgObject UlgObject;

/**
 * Create a new class context, usable to retrieve classes by name.
 *
 * @return The newly created class context.
 */
UlgContext* ulg_context_new();

/**
 * Destroy a UlgContext, and all it's registered classes.
 *
 * @param context The context to destroy.
 */
void ulg_context_free(UlgContext* context);

/** Callback defining a type. The adress of the callback will be used as type id. */
typedef const UlgClass* (UlgClassDefinition)(UlgClassFactory*);

/**
 * Register a class in the class context, using the given definition.
 *
 * It will assert if the class is already registered in the context.
 *
 * @param context    A previously created UlgContext.
 * @param definition A class definition callback.
 *
 * @return The newly created class.
 */

void ulg_class_register(UlgContext* context, UlgClassDefinition definition);

/**
 * Get a class using it's definition as a key.
 *
 * If the class wasn't registered, the function will return NULL.
 *
 * @param context   A previously created UlgContext.
 * @param definition A class definition callback.
 *
 * @return The newly created class.
 */

const UlgClass* ulg_class_get(UlgContext* context, UlgClassDefinition definition);

/**
 * Retrieve a class by it's type name.
 *
 * @param context A previously created and initialized UlgContext.
 * @param name    The class name to get.
 *
 * @return The matching class, or NULL if it was not found in the context.
 */

const UlgClass* ulg_class_get_by_name(UlgContext* context, const char* name);

/**
 * Create a new class object.
 *
 * This method is meant to be called in a UlgClassDefinition callback only.
 *
 * @param factory           Opaque type passed to type definitions functions by the type context.
 * @param name              Name of the new class.
 * @param data_size         Size of the class data.
 * @param parent_definition Parent class definition function.
 * @param vtable_size       Size of the class virtual table. If 0 is given, the vtable of the parent
 *                          will be copied.
 */
UlgClass* ulg_class_declare(
    UlgClassFactory* factory,
    const char* name,
    size_t data_size,
    UlgClassDefinition parent_definition,
    size_t vtable_size
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
void* ulg_class_edit_vtable(UlgClass* class_);

/** Callback for UlgObjects property getter.*/
typedef UlgValue (*UlgGetter)(const UlgObject*);

/** Callback for UlgObjects property setter.*/
typedef void (*UlgSetter)(UlgObject*, UlgValue);

/**
 * Add a property to this class.
 * @param class_  Add a property to this class.
 * @param name    The name of the property.
 * @param setter  Callback that get the property value.
 * @param getter  Callback that set the property value.
 */
void ulg_class_add_property(UlgClass* class_, const char* name, UlgGetter getter, UlgSetter setter);

/**
 * UlgObject virtual table structure. See ulg_class_edit_vtable to implement methods on
 * class inheriting from UlgObject.
 */
typedef struct _UlgObjectVT {
    /** Called after the given object is allocated and initialized. */
    void (*init)(UlgObject*);

    /** Called after the given object is allocated and initialized. */
    void (*cleanup)(UlgObject*);
} UlgObjectVT;

/**
 * @brief UlgObject class definition.
 */
const UlgClass* ulg_object_type(UlgClassFactory* factory);

/**
 * Create an object of the given class.
 */
UlgObject* ulg_object_new(const UlgClass* class_);

/**
 * Release the given UlgObject.
 */
void ulg_object_free(UlgObject* object);

/**
 * @brief Return the virtual table of this object.
 * 
 * @param object Get the virtual table for this object.
 * 
 * @return A pointer to the object's virtual table.
 */
const void* ulg_object_vtable(const UlgObject* object);

/**
 * Get a property from an object.
 *
 * @param object        The target object on which to set the property.
 * @param property_name Name of the property to set.
 * @return              The property value.
 */
UlgValue ulg_object_get(const UlgObject* object, const char* property_name);

/**
 * Set a property on an object.
 *
 * @param object        The target object on which to set the property.
 * @param property_name Name of the property to set.
 * @param value         Value to set the property to.
 */
void ulg_object_set(UlgObject* object, const char* property_name, UlgValue value);
