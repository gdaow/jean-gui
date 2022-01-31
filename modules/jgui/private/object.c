/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include <stdalign.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "common/debug.h"
#include "class.h"
#include "jgui/class.h"

typedef struct jg_object_header_s {
    const jg_class* class_;
    max_align_t object[];
} jg_object_header;

const char* jg_object_class_id = "Object";

void* jg_object_new(const jg_class* class_) {
    JG_ASSERT(class_ != NULL);

    size_t total_size = sizeof(jg_object_header) + class_->size;

    jg_object_header* header = calloc(1, total_size);
    JG_ASSERT(header != NULL); // TODO(corentin@ki-dour.org) handle error.

    header->class_ = class_;
    void* object = header + 1;

    while(class_) {
        jg_constructor constructor = class_->constructor;
        if(constructor) {
            constructor(object);
            break;
        }
        class_ = class_->parent;
    }

    return object;
}

void jg_object_free(void* object) {
    JG_ASSERT(object != NULL);
    jg_object_header* header = (jg_object_header*)object - 1;
    const jg_class* class_ = header->class_;

    while(class_) {
        jg_destructor destructor = class_->destructor;
        if(destructor) {
            destructor(object);
            break;
        }
        class_ = class_->parent;
    }

    free(header);
}

static const jg_class* get_class(const void* object);

jg_value jg_object_get(const void* object, const char* property_id) {
    JG_ASSERT(object != NULL);
    JG_ASSERT(property_id != NULL && strlen(property_id) > 0);

    const jg_class* class_ = get_class(object);
    const jg_member* member = jg_class_get_member(class_, property_id, JG_MEMBER_PROPERTY);
    JG_ASSERT(member != NULL); // TODO(corentin@ki-dour.org) handle error.
    return member->data.property.getter(object);
}

void jg_object_set(void* object, const char* property_id, const jg_value value) {
    JG_ASSERT(object != NULL);
    JG_ASSERT(property_id != NULL && strlen(property_id) > 0);

    const jg_class* class_ = get_class(object);
    const jg_member* member = jg_class_get_member(class_, property_id, JG_MEMBER_PROPERTY);
    JG_ASSERT(member != NULL); // TODO(corentin@ki-dour.org) handle error.
    member->data.property.setter(object, value);
}

jg_value jg_object_call(const void* object, const char* method_id, jg_value* arguments) {
    JG_ASSERT(object != NULL);
    JG_ASSERT(method_id != NULL && strlen(method_id) > 0);
    JG_ASSERT(arguments != NULL);

    const jg_class* class_ = get_class(object);
    const jg_member* member = jg_class_get_member(class_, method_id, JG_MEMBER_METHOD);
    JG_ASSERT(member != NULL); // TODO(corentin@ki-dour.org) handle error.
    return member->data.method(NULL);
}

static const jg_class* get_class(const void* object) {
    jg_object_header* header = (jg_object_header *)object - 1;
    return header->class_;
}
