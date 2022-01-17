#pragma once
#include <stddef.h>

typedef struct _UlgClassRegistry UlgClassRegistry;
typedef struct _UlgClassFactory UlgClassFactory;
typedef struct _UlgClass UlgClass;

/** Callback defining a type. The adress of the callback will be used as type id. */
typedef const UlgClass* (UlgClassDefinition)(UlgClassFactory*);

/**
 * Create a new class registry, usable to retrieve classes by name.
 *
 * @return The newly created class registry.
 */
UlgClassRegistry* ulg_class_registry_new();

/**
 * Destroy a UlgClassRegistry, and all it's registered classes.
 *
 * @param registry The registry to destroy.
 */
void ulg_class_registry_free(UlgClassRegistry* registry);

/**
 * Register a class in the class registry, using the given definition.
 *
 * If the class was already registered, it will return the previously created class.
 *
 * @param registry   A previously created UlgClassRegistry.
 * @param definition A class definition callback.
 *
 * @return The newly created class.
 */

const UlgClass* ulg_class_get(UlgClassRegistry* registry, UlgClassDefinition definition);

/**
 * Create a new class object.
 *
 * This method is meant to be called in a UlgClassDefinition callback only.
 *
 * @param factory           Opaque type passed to type definitions functions by the type registry.
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
