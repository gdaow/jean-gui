/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "jgui/private/object-model/class.h"

#include <stdalign.h>
#include <stdbool.h>

#include <jgui/object-model/value.h>

#include "jgui/object-model/class.h"
#include "jgui/private/memory/string.h"
#include "jgui/private/misc/assert.h"
#include "private/containers/index.h"
#include "private/object-model/member.h"

struct jg_class_s {
    jg_index member_index;
    const jg_class* parent;
    size_t size;
};

void jg_class_add_method(jg_class* class_, const char* name, jg_method method) {
    assert(class_ != NULL);
    assert(!jg_null_or_empty(name));
    assert(method != NULL);

    //TODO(corentin@ki-dour.org) : handle case where member of the same name is already
    //registered
    jg_member* member = jg_index_add(&class_->member_index, name);
    jg_method_init(member, method);
}

void jg_class_add_property(jg_class* class_, const char* name, jg_getter getter, jg_setter setter) {
    assert(class_ != NULL);
    assert(!jg_null_or_empty(name));
    assert(getter != NULL);
    assert(setter != NULL);

    //TODO(corentin@ki-dour.org) : handle case where member of the same name is already
    //registered
    jg_member* member = jg_index_add(&class_->member_index, name);
    jg_property_init(member, getter, setter);

}

void jg_class_init(jg_class* class_, const jg_class* parent, size_t size) {
    *class_ = (jg_class) {
        .member_index = {0},
        .parent = parent,
        .size = size
    };
}

void jg_class_cleanup(void* class_) {
    jg_index_cleanup(&((jg_class*)class_)->member_index, jg_member_cleanup);
}
