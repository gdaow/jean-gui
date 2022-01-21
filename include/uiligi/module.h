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

static const size_t MAX_NAME_LENGTH = 256;

typedef struct _UlgClass UlgClass;
typedef struct _UlgClassDefinition UlgClassDefinition;
typedef struct _UlgModule UlgModule;
typedef struct _UlgModuleDefinition UlgModuleDefinition;
typedef struct _UlgMember UlgMember;

/**
 * Create a new module definition.
 *
 * @return The newly created class module.
 */
UlgModuleDefinition* ulg_module_new();

/** Callback defining a type. The adress of the callback will be used as type id. */
typedef void (UlgClassDeclaration)(UlgClassDefinition*);

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

void ulg_class_register(UlgModuleDefinition* module, UlgClassDeclaration class_);

/**
 * Create a usable UlgModule from a UlgModuleDefinition
 * 
 * @param definition 
 * @return UlgModule* 
 */
UlgModule* ulg_module_build(UlgModuleDefinition* module);

/**
 * Destroy a UlgModule, and all it's registered classes.
 *
 * @param module The module to destroy.
 */
void ulg_module_free(UlgModule* module);

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

const UlgClass* ulg_class_get(const UlgModule* module, UlgClassDeclaration class_);

/**
 * Retrieve a class by it's type name.
 *
 * @param module A previously created and initialized UlgModule.
 * @param name    The class name to get.
 *
 * @return The matching class, or NULL if it was not found in the module.
 */

const UlgClass* ulg_class_get_by_name(UlgModule* module, const char* name);
