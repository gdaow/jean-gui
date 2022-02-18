/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "jgui/private/object-model/module.h"

#include <string.h>

#include "jgui/private/containers/index.h"
#include "jgui/private/memory/alloc.h"
#include "jgui/private/memory/string.h"
#include "jgui/private/misc/assert.h"
#include "jgui/private/object-model/class.h"

struct jg_module_s {
    jg_index class_index;
};

void jg_module_init(jg_module* module) {
    *module = (jg_module) {0};
}

void jg_module_cleanup(jg_module* module) {
    jg_index_cleanup(&module->class_index, jg_class_cleanup);
}

jg_class* jg_module_add_class(
    jg_module* module,
    const char* name,
    const jg_class* parent,
    size_t size
) {
    assert(module != NULL);
    assert(!jg_null_or_empty(name));
    assert(size > 0);
    //TODO(corentin@ki-dour.org) : handle case where class is already registered
    jg_class* class_ = jg_index_add(&module->class_index, name, NULL);
    jg_class_init(class_, parent, size);
    return class_;
}

const jg_class* jg_module_get_class(const jg_module* module, const char* name) {
    assert(module != NULL);
    assert(!jg_null_or_empty(name));
    assert(name != NULL && strlen(name) > 0);

    return jg_index_get(&module->class_index, name);
}
