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

typedef struct jg_object_header_s {
    const jg_class* class_;
    alignas(max_align_t) void* object;
} jg_object_header;

const char* jg_object_class_id = "Object";

void* jg_object_new(jg_class* class_) {
    JG_ASSERT(class_ != NULL);
    size_t total_size = sizeof(jg_object_header) + class_->size;
    jg_object_header* header = calloc(1, total_size);
    header->class_ = class_;
    JG_ASSERT(header != NULL); // TODO(corentin@ki-dour.org) handle error.
    return header + 1;
}

void jg_object_free(void* object) {
    JG_ASSERT(object != NULL);

    jg_object_header* header = object;
    free(header - 1);
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
    return member->data.method(arguments);
}

static const jg_class* get_class(const void* object) {
    jg_object_header* header = (jg_object_header *)object - 1;
    return header->class_;
}
