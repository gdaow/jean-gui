/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <stdalign.h>

#include "common/constants.h"
#include "common/debug.h"
#include "common/index.h"
#include "class.h"
#include "module.h"

void jg_class_set_constructor(jg_class_definition* class_definition, jg_constructor constructor, jg_destructor destructor) {
    class_definition->constructor = constructor;
    class_definition->destructor = destructor;
}


// Add a previously constructed member definition to a class definition.
static void add_member(
    jg_class_definition* class_definition,
    const char* id,
    const jg_member_definition* member_definition
);

void jg_class_add_method(jg_class_definition* class_definition, const char* id, jg_method method) {
    JG_ASSERT(class_definition != NULL);
    JG_ASSERT(id != NULL);
    JG_ASSERT(method != NULL);

    add_member(class_definition, id, &(jg_member_definition) {
        .type = JG_MEMBER_METHOD,
        .data.method = method
    });
}

void jg_class_add_property(
    jg_class_definition* class_definition,
    const char* id,
    jg_getter_t getter,
    jg_setter_t setter
) {
    JG_ASSERT(class_definition != NULL);
    JG_ASSERT(id != NULL);
    JG_ASSERT(getter != NULL);
    JG_ASSERT(setter != NULL);

    add_member(class_definition, id, &(jg_member_definition) {
        .type = JG_MEMBER_PROPERTY,
        .data.property.getter = getter,
        .data.property.setter = setter
    });
}

static const char* get_class_id_and_next(const void** item);
static void build_class(const void* item, int sorted_id, void* user_data);

jg_index jg_class_build_index(const jg_class_definition* first_class_definition, jg_pool* pool) {
    JG_ASSERT(first_class_definition != NULL);
    JG_ASSERT(pool != NULL);

    jg_index class_index = jg_index_build(
        first_class_definition,
        get_class_id_and_next,
        build_class,
        &pool->indexes,
        &pool->ids,
        pool
    );

    jg_class* class_array = pool->classes;
    size_t class_count = class_index.count;

    // resolve hierarchy
    const jg_class_definition* current_definition = first_class_definition;
    while(current_definition) {
        const char* parent_string_id = current_definition->parent_id;

        if(parent_string_id != NULL) {
            int child_id = jg_index_search(&class_index, current_definition->id);
            JG_ASSERT(child_id >= 0 && (size_t)child_id < class_count);

            int parent_id = jg_index_search(&class_index, parent_string_id);

            //TODO(corentin@ki-dour.org) handle error (parent class not found)
            JG_ASSERT(parent_id >= 0 && (size_t)parent_id < class_count);
            
            class_array[child_id].parent = &class_array[parent_id];
        }

        current_definition = current_definition->next_class;
    }

    pool->classes += class_index.count;

    return class_index;
}

const jg_member* jg_class_get_member(const jg_class* class_, const char* id, jg_member_type type) {
    JG_ASSERT(class_ != NULL);
    JG_ASSERT(id != NULL);

    while(class_) {
        int member_id = jg_index_search(&(class_->member_index), id);

        if(member_id >= 0) {
            jg_member* candidate = &(class_->member_array[member_id]);
            if(candidate->type == type) {
                return candidate;
            }

            JG_ASSERT(false);// TODO(corentin@ki-dour.org): handle error
        }
        class_ = class_->parent;
    }

    JG_ASSERT(false);// TODO(corentin@ki-dour.org): handle error
    return NULL;
}

static void add_member(
    jg_class_definition* class_definition,
    const char* id,
    const jg_member_definition* member_definition
) {
    JG_ASSERT(class_definition != NULL);
    JG_ASSERT(id != NULL);
    JG_ASSERT(member_definition != NULL);

    jg_member_definition* new_member = jg_arena_alloc(
        class_definition->arena,
        sizeof(jg_member_definition),
        alignof(jg_member_definition)
    );
    JG_ASSERT(new_member != NULL); // TODO(corentin@ki-dour.org) handle error.

    *new_member = *member_definition;
    new_member->id = jg_copy_identifier(class_definition->arena, id);
    JG_ASSERT(new_member->id != NULL); // TODO(corentin@ki-dour.org) handle error.
    new_member->next_member = class_definition->first_member;
    class_definition->first_member = new_member;
}

static const char* get_member_id_and_next(const void** item) {
    JG_ASSERT(item != NULL);
    JG_ASSERT(*item != NULL);

    const jg_member_definition* member_definition = *item;
    const char* id = member_definition->id;
    *item = member_definition->next_member;
    return id;
}

static void build_member(const void* item, int sorted_id, void* user_data) {
    JG_ASSERT(item != NULL);
    JG_ASSERT(sorted_id >= 0);
    JG_ASSERT(user_data != NULL);

    const jg_member_definition* member_definition = item;
    jg_pool* pool = user_data;
    pool->members[sorted_id] = (jg_member) {
        .type = member_definition->type,
        .data = member_definition->data
    };
}

static const char* get_class_id_and_next(const void** item) {
    JG_ASSERT(item != NULL);
    JG_ASSERT(*item != NULL);

    const jg_class_definition* class_definition = *item;
    const char* id = class_definition->id;
    *item = class_definition->next_class;
    return id;
}

static jg_index build_member_index(const jg_member_definition* first_member, jg_pool* pool) {
    JG_ASSERT(pool != NULL);

    return jg_index_build(
        first_member,
        get_member_id_and_next,
        build_member,
        &pool->indexes,
        &pool->ids,
        pool
    );
}

static void build_class(const void* item, int sorted_id, void* user_data) {
    JG_ASSERT(item != NULL);
    JG_ASSERT(sorted_id >= 0);
    JG_ASSERT(user_data != NULL);

    jg_pool* pool = user_data;
    jg_class* class_pool = pool->classes;
    const jg_class_definition* class_definition = item;

    jg_class* new_class = &class_pool[sorted_id];

    *new_class = (jg_class) {
        .size = class_definition->size,
        .align = class_definition->align,
        .member_index = build_member_index(class_definition->first_member, pool),
        .member_array = pool->members,
        .constructor = class_definition->constructor,
        .destructor = class_definition->destructor
        
    };

    pool->members += new_class->member_index.count;
}
