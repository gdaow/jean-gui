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

typedef struct _UlgClass UlgClass;
typedef struct _UlgClassDefinition UlgClassDefinition;

typedef void (UlgClassDeclaration)(UlgClassDefinition*);

/**
 * Create a new class object.
 *
 * This method is meant to be called in a UlgClassDeclaration callback only.
 *
 * @param definition           Opaque type passed to type definitions functions by the type module.
 * @param name              Name of the new class.
 * @param data_size         Size of the class data.
 * @param parent_definition Parent class definition function.
 * @param vtable_size       Size of the class virtual table. If 0 is given, the vtable of the parent
 *                          will be copied.
 */
void ulg_class_define(UlgClassDefinition* definition, const char* name, UlgClassDeclaration parent);

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
void ulg_class_add_property(UlgClassDefinition* definition, const char* name, UlgGetter getter, UlgSetter setter);
