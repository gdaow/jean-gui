#pragma once

#include <uiligi/value.h>

typedef struct UlgObject UlgObject;
typedef struct UlgClass UlgClass;

/** Callback for UlgObjects private state initialization.*/
typedef void (*UlgInitialize)(void*);

/** Callback for UlgObjects private state initialization.*/
typedef void (*UlgCleanup)(void*);

/** Callback for UlgObjects property getter.*/
typedef UlgValue (*UlgGetter)(const UlgObject*, const void*);

/** Callback for UlgObjects property setter.*/
typedef void (*UlgSetter)(UlgObject*, void*, UlgValue);

/**
 * Create a new class object.
 * @param name The name of the class.
 * @param initializer Callback called to initialize data for objects of this class.
 * @param cleaner  Callback called to release data for objects of this class.
 */
UlgClass* ulg_class_create(
    const char* name,
    size_t size,
    UlgInitialize initialize,
    UlgCleanup cleanup,
    const UlgClass* parent
);

/**
 * Destroy a UlgClass, and all it's associated properties.
 */
void ulg_class_destroy(UlgClass* class_);

/**
 * Add a property to this class.
 * @param class_  Add a property to this class.
 * @param name    The name of the property.
 * @param setter  Callback that get the property value.
 * @param getter  Callback that set the property value.
 */
void ulg_class_add_property(UlgClass* class_, const char* name, UlgGetter getter, UlgSetter setter);

/**
 * Create an object of the given class.
 */
UlgObject* ulg_object_create(const UlgClass* class_);

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
UlgValue ulg_object_get(const UlgObject* object, const char* property_name);
