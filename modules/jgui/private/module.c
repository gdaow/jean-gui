/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <stdalign.h>
#include <string.h>
#include <stdlib.h>

#include <jgui/module.h>

#include "common/arena.h"
#include "common/debug.h"
#include "common/index.h"
#include "common/memory.h"
#include "common/string.h"
#include "class.h"
#include "context.h"
#include "module.h"

jg_class_builder* jg_module_add_class(
    jg_module_builder* module_builder,
    const char* id,
    const char* parent_namespace,
    const char* parent_id,
    size_t size
) {
    JG_ASSERT(module_builder != NULL);
    JG_ASSERT(id != NULL && strlen(id) > 0);
    JG_ASSERT(parent_id == NULL || strlen(parent_id) > 0);

    jg_allocator* allocator = module_builder->allocator;
    char* parent_id_copy = NULL;
    char* parent_namespace_copy = NULL;
    
    if(parent_id) {
        parent_id_copy = jg_copy_identifier(allocator, parent_id);
        if(parent_namespace == NULL) {
            // Use current module namespace if none is provided.
            parent_namespace = module_builder->namespace;
        }
        parent_namespace_copy = jg_copy_identifier(allocator, parent_namespace);

        JG_ASSERT(parent_id_copy != NULL); // TODO(corentin@ki-dour.org) handle error.
    }

    jg_class_builder* new_class = jg_allocate_aligned(
        allocator,
        sizeof(jg_class_builder),
        alignof(jg_class_builder)
    );

    JG_ASSERT(new_class != NULL); // TODO(corentin@ki-dour.org) handle error.
    
    *new_class = (jg_class_builder) {
        .allocator = module_builder->allocator,
        .id = jg_copy_identifier(allocator, id),
        .parent_id = parent_id_copy,
        .parent_namespace = parent_namespace_copy,
        .size = size,
        .next_class = module_builder->first_class,
        .first_member = 0
    };

    module_builder->first_class = new_class;

    return new_class;
}

static const char* get_module_id_and_next(const void** item);
static void build_module(const void* item, int sorted_id, void* user_data);

jg_index jg_module_build_index(jg_module_builder* first_module_builder, jg_pool* pool) {
    JG_ASSERT(first_module_builder != NULL);
    JG_ASSERT(pool != NULL);

    jg_index module_index = jg_index_build(
        first_module_builder,
        get_module_id_and_next,
        build_module,
        &pool->indexes,
        &pool->ids,
        pool
    );

    pool->modules += module_index.count;

    return module_index;
}

jg_class* jg_module_get_class(jg_module* module, const char* id) {
    JG_ASSERT(module != NULL);
    JG_ASSERT(id != NULL && strlen(id) > 0);

    int class_id = jg_index_search(&module->class_index, id);
    if(class_id == -1) {
        return NULL;
    }

    return &(module->class_array[class_id]);
}

static const char* get_module_id_and_next(const void** item) {
    JG_ASSERT(item != NULL);
    JG_ASSERT(*item != NULL);

    const jg_module_builder* module_builder = *item;
    const char* id = module_builder->namespace;
    *item = module_builder->next_module;
    return id;
}

static void build_module(const void* item, int sorted_id, void* user_data) {
    JG_ASSERT(item != NULL);
    JG_ASSERT(sorted_id >= 0);
    JG_ASSERT(user_data != NULL);

    jg_pool* pool = user_data;
    jg_module* module_pool = pool->modules;
    const jg_module_builder* module_builder = item;

    jg_module* new_module = &module_pool[sorted_id];
    jg_class* class_array = pool->classes;

    *new_module = (jg_module) {
        .class_index = jg_class_build_index(module_builder->first_class, pool),
        .class_array = class_array
    };
}
