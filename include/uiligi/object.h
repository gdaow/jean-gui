#pragma once

typedef struct UlgObject UlgObject;
typedef struct UlgClass UlgClass;

/**
 * Create an object of the given class.
 */
UlgObject* ulg_object_create(UlgClass* ulg_class);

/**
 * Release the given UlgObject.
 */
void ulg_object_destroy(UlgObject* object);
