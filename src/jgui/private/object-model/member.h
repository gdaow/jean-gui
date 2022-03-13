/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 * Private declarations for jgui/include/object-model/member.h.
 * See public include for documentation.
 *
 */
#ifndef JGUI_PRIVATE_OBJECT_MODEL_MEMBER_H
#define JGUI_PRIVATE_OBJECT_MODEL_MEMBER_H

#include <jgui/object-model/member.h>
#include <jgui/misc/utils.h>

struct jg_member_s;

jg_forward_declare(jg_member);

/**
 * Initialize a member as a method wrapping the given function pointer.
 *
 * @param method Pointer to a function this member is bound to.
 *
 * @return The wrapped int value.
 */
void jg_method_init(jg_member* member, jg_method method);

/**
 * Call the given member with the given arguments.
 *
 * If the member isn't a method, the behavior is undefined.
 *
 * @param arguments Arguments to call the method with.
 *
 * @return The wrapped return value of the method call.
 */
jg_value jg_method_call(const jg_member* method, jg_arguments* arguments);

/**
 * Initialize a member as a property using the given getter and setter.
 *
 * @param getter Pointer to the property getter.
 * @param setter Pointer to the property setter.
 */
void jg_property_init(jg_member* member, jg_getter getter, jg_setter setter);

/**
 * Get the value of the given property on the given object.
 *
 * If the member isn't a property, the behavior is undefined.
 *
 * @param object Object to get the property on.
 *
 * @return The property value.
 */
jg_value jg_property_get(const jg_member* property, void* object);

/**
 * Set the value of the given property on the given object.
 *
 * If the member isn't a property, the behavior is undefined.
 *
 * @param object Object to set the property on.
 * @param value  The property value to set.
 */
void jg_property_set(const jg_member* property, void* object, jg_value value);

/**
 * Releases memory used by a member.
 */
void jg_member_cleanup(jg_member* member);

typedef struct jg_member_s {
    enum {
        JG_MEMBER_PROPERTY,
        JG_MEMBER_METHOD
    } _type;
    union {
        struct {
            jg_getter _getter;
            jg_setter _setter;
        } _property;
        jg_method _method;
    } _data;
} jg_member;

#endif

