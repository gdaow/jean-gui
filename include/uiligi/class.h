#pragma once

#include "uiligi/value.h"

typedef struct UlgObject UlgObject;
typedef struct UlgClass UlgClass;

typedef void* (*UlgConstructor)();
typedef void (*UlgDestructor)(void*);

typedef UlgValue (*UlgGetter)(void*);
typedef void (*UlgSetter)(void*, UlgValue);

/**
 * Create a new class object.
 * @param name The name of the class.
 * @param constructor Callback called to initialize data for objects of this class.
 * @param destructor  Callback called to release data for objects of this class.
 */
UlgClass* ulg_class_create(const char* name, UlgConstructor constructor, UlgDestructor destructor);

/**
 * Add a property to this class.
 * @param ulg_class Add a property to this class.
 * @param name      The name of the property.
 * @param setter    Callback that get the property value.
 * @param getter    Callback that set the property value.
 */
void ulg_class_add_property(UlgClass* ulg_class, const char* name, UlgGetter getter, UlgSetter setter);
