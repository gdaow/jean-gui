#pragma once

#include <uiligi/value.h>

typedef struct _UlgClass UlgClass;
typedef struct _UlgObject UlgObject;

/** Callback for UlgObjects property getter.*/
typedef UlgValue (*UlgGetter)(const UlgObject*);

/** Callback for UlgObjects property setter.*/
typedef void (*UlgSetter)(UlgObject*, UlgValue);

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

/**
 * Add a property to this class.
 * @param class_  Add a property to this class.
 * @param name    The name of the property.
 * @param setter  Callback that get the property value.
 * @param getter  Callback that set the property value.
 */
void ulg_class_add_property(UlgClass* class_, const char* name, UlgGetter getter, UlgSetter setter);
