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
#include "module.h"
#include "class.h"

struct jg_module_s {
    jg_index class_index;
    jg_pool pool;
};

jg_module_definition* jg_module_new() {
    jg_arena* arena = jg_arena_new(0);
    jg_module_definition* new_module = jg_arena_alloc(
        arena,
        sizeof(jg_module_definition),
        alignof(jg_module_definition)
    );

    JG_ASSERT(new_module != NULL); // TODO(corentin@ki-dour.org) handle error.

    *new_module = (jg_module_definition) { 0 };
    new_module->arena = arena;
    jg_module_add_class(new_module, "Object", NULL, 0, 1);

    return new_module;
}

jg_class_definition* jg_module_add_class(
    jg_module_definition* module_definition,
    const char* id,
    const char* parent_id,
    size_t size,
    size_t align
) {
    JG_ASSERT(module_definition != NULL);
    JG_ASSERT(id != NULL && strlen(id) > 0);
    JG_ASSERT(parent_id == NULL || strlen(parent_id) > 0);
    JG_ASSERT(align > 0);

    jg_arena* arena = module_definition->arena;
    char* parent_id_copy = NULL;
    
    if(parent_id) {
        parent_id_copy = jg_copy_identifier(arena, parent_id);
        JG_ASSERT(parent_id_copy != NULL); // TODO(corentin@ki-dour.org) handle error.
    }

    jg_class_definition* new_class = jg_arena_alloc(
        arena,
        sizeof(jg_class_definition),
        alignof(jg_class_definition)
    );
    JG_ASSERT(new_class != NULL); // TODO(corentin@ki-dour.org) handle error.
    
    *new_class = (jg_class_definition) {
        .arena = module_definition->arena,
        .id = jg_copy_identifier(arena, id),
        .parent_id = parent_id_copy,
        .size = size,
        .align = align,
        .next_class = module_definition->first_class,
        .first_member = 0
    };

    module_definition->first_class = new_class;

    return new_class;
}

static jg_pool create_pool(jg_module_definition* module_definition);

jg_module* jg_module_build(jg_module_definition* module_definition) {
    JG_ASSERT(module_definition != NULL);

    jg_pool pool = create_pool(module_definition);
    jg_pool pool_copy = pool; // we keep the pool's buffers index copy, as jg_class_build_index will increment them.

    jg_index class_index = jg_class_build_index(module_definition->first_class, &pool);

    jg_arena_free(module_definition->arena);

    jg_module* result = malloc(sizeof(jg_module));
    JG_ASSERT(result != NULL); // TODO(corentin@ki-dour.org) handle error.
    *result = (jg_module) {
        .class_index = class_index,
        .pool = pool_copy
    };

    return result;
}

void jg_module_free(jg_module* module) {
    JG_ASSERT(module != NULL);

    free(module->pool.classes);
    free(module->pool.indexes);
    free(module->pool.members);
    free(module->pool.ids);
    free(module);
}

const jg_class* jg_module_get_class(const jg_module* module, const char* id) {
    JG_ASSERT(module != NULL);
    JG_ASSERT(id != NULL && strlen(id) > 0);

    int class_id = jg_index_search(&module->class_index, id);
    if(class_id == -1) {
        return NULL;
    }

    return &(module->pool.classes[class_id]);
}

static jg_pool create_pool(jg_module_definition* module) {
    JG_ASSERT(module != NULL);

    jg_class_definition* class_ = module->first_class;
    size_t class_count = 0;
    size_t member_count = 0;
    size_t id_length = 0;

    while(class_) {
        ++class_count;
        id_length += strlen(class_->id) + 1; // for terminal \0
        jg_member_definition* member = class_->first_member;
        while(member) {
            ++member_count;
            id_length += strlen(member->id) + 1; // for terminal \0
            member = member->next_member;
        }
        class_ = class_->next_class;
    }

    // TODO(corentin@ki-dour.org): handle errors. (Will crash if we try to create an empty module, or empty classes).
    JG_ASSERT(class_count > 0);
    JG_ASSERT(member_count > 0);
    JG_ASSERT(id_length > 0);

    jg_pool result = {
        .classes = calloc(class_count, sizeof(jg_class)),
        .indexes = calloc(class_count + member_count, sizeof(char*)),
        .members = calloc(member_count, sizeof(jg_member)),
        .ids = calloc(id_length, sizeof(char))
    };

    // TODO(corentin@ki-dour.org): handle error.
    JG_ASSERT(result.classes != NULL);
    JG_ASSERT(result.indexes != NULL);
    JG_ASSERT(result.members != NULL);
    JG_ASSERT(result.ids != NULL);

    return result;
}
