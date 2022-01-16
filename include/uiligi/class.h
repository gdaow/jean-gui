#pragma once

#include <uiligi/value.h>

typedef struct UlgObject UlgObject;
typedef struct UlgClass UlgClass;

typedef void (*UlgInitialize)(void*);
typedef void (*UlgCleanup)(void*);

typedef UlgValue (*UlgGetter)(UlgObject*, void*);
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
    UlgClass* parent
);

/**
 * Add a property to this class.
 * @param ulg_class Add a property to this class.
 * @param name      The name of the property.
 * @param setter    Callback that get the property value.
 * @param getter    Callback that set the property value.
 */
void ulg_class_add_property(UlgClass* ulg_class, const char* name, UlgGetter getter, UlgSetter setter);
