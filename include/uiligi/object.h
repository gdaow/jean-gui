#pragma once

#include <uiligi/value.h>

typedef struct _UlgClass UlgClass;
typedef struct _UlgClassFactory UlgClassFactory;
typedef struct _UlgObject UlgObject;

typedef struct _UlgObjectVT {
    void (*init)(UlgObject*);
    void (*cleanup)(UlgObject*);
} UlgObjectVT;

/**
 * @brief UlgObject class definition.
 */
const UlgClass* ulg_object(UlgClassFactory* factory);

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
