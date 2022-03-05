/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 * TODO:
 *  -document
 */
#ifndef JGUI_PRIVATE_OBJECT_MODEL_MEMBER_H
#define JGUI_PRIVATE_OBJECT_MODEL_MEMBER_H

#include <jgui/object-model/member_types.h>
#include <jgui/misc/utils.h>

jg_forward_declare(jg_member)

void jg_method_init(jg_member* member, jg_method method);

void jg_property_init(jg_member* member, jg_getter getter, jg_setter setter);

jg_value jg_property_get(void* object, jg_member* member);

void jg_property_set(void* object, jg_member* member, jg_value value);

void jg_member_cleanup(void* member);

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

