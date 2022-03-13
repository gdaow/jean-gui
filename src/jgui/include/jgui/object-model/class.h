/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * A class is a type definition, aggregating member declaration and informations
 * nedded to instanciate objects. Class are not created directly, but rather
 * registered in pluginsn during context context loading, by calling
 * jg_context_add_class. Method and properties can be called / get /set using
 * jg_object_call, jg_object_get and jg_object_set methods.
 *
 */
#ifndef JGUI_OBJECT_MODEL_CLASS_H
#define JGUI_OBJECT_MODEL_CLASS_H

#include <jgui/misc/utils.h>
#include <jgui/object-model/member.h>

jg_forward_declare(jg_class)
jg_forward_declare(jg_arguments)

/**
 * Add a method to this class.
 *
 * If a property or a method with the same name is already registered in this
 * class, the behavior is undefined.
 *
 * @param name   Name used to identify this method.
 * @param method Pointer to the method function to bind to the method name.
 */
void jg_class_add_method(jg_class* class_, const char* name, jg_method method);

/**
 * Add a property to this class.
 *
 * If a property or a method with the same name is already registered in this
 * class, the behavior is undefined.
 *
 * @param name   Name used to identify this property.
 * @param getter Pointer to the getter function to bind to this property name.
 * @param setter Pointer to the setter function to bind to this property name.
 */
void jg_class_add_property(jg_class* class_, const char* name, jg_getter getter, jg_setter setter);

#endif

