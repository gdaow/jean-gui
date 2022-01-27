/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <assert.h>
#include <stdalign.h>
#include <stdlib.h>
#include <string.h>

#include <jg/object.h>

#include "private/memory.h"
#include "private/misc.h"

struct jg_module_definition_s {
    jg_arena* allocator;
    jg_class_definition* class_definitions;
    size_t class_count;

    // total number of members to allocate for all classes of this module.
    size_t member_count;

    // used to accumulate the total length of class & member names, in order to allocate a big buffer for them all
    // when building modules.
    size_t names_length; 
};

typedef struct jg_member_definition_s jg_member_definition;

struct jg_class_definition_s {
    jg_arena* allocator;
    const char* name;
    char* parent;
    size_t size;
    size_t align;
    jg_class_definition* next;
    jg_member_definition* members;
    size_t member_count;
    size_t* total_member_count;
    size_t* total_names_length; // total size of classes and member names.
};

struct jg_module_s {
    jg_index class_index;
    jg_class* class_array;
    jg_member* member_pool;
    const char** index_pool;
    char* string_pool;
};

struct jg_class_s {
    jg_index member_index;
    const jg_class* parent;
    jg_member* member_array;
    size_t size;
    size_t align;
};

struct jg_member_s {
    jg_getter_t getter;
    jg_setter_t setter;
};

jg_module_definition* jg_module_new() {
    jg_arena* allocator = jg_arena_new(0);
    jg_module_definition* module = JG_ARENA_ALLOC_INIT(
        allocator,
        jg_module_definition,
        &(jg_module_definition) {
            .allocator = allocator,
            .class_definitions = NULL,
            .class_count = 0,
            .member_count = 0,
            .names_length = 0,
        }
    );
    jg_class_new(module, "Object", NULL, 0, 1);
    return module;
}

jg_class_definition* jg_class_new(
    jg_module_definition* module,
    const char* name,
    const char* parent,
    size_t size,
    size_t align
) {
    jg_arena* allocator = module->allocator;
    size_t* names_length = &module->names_length;
    ++module->class_count;
    char* parent_copy = NULL;
    
    if(parent) {
        parent_copy = jg_arena_strcpy(allocator, parent, MAX_NAME_LENGTH, NULL);
    }

    jg_class_definition* new_class = JG_ARENA_ALLOC_INIT(
        allocator,
        jg_class_definition,
        &(jg_class_definition) {
            .allocator = module->allocator,
            .name = jg_arena_strcpy(allocator, name, MAX_NAME_LENGTH, names_length),
            .parent = parent_copy,
            .size = size,
            .align = align,
            .next = module->class_definitions,
            // We don't accumulate the length of the parent in the total space needed for names,
            // as the parent class will do it.
            .members = NULL,
            .member_count = 0,
            .total_member_count = &module->member_count,
            .total_names_length = names_length,
        }
    );

    module->class_definitions = new_class;
    return new_class;
}

struct jg_member_definition_s {
    const char* name;
    jg_member_definition* next;
    jg_getter_t getter;
    jg_setter_t setter;
};

void jg_class_add_property(jg_class_definition* class_, const char* name, jg_getter_t getter, jg_setter_t setter) {
    class_->members = JG_ARENA_ALLOC_INIT(
        class_->allocator,
        jg_member_definition,
        &(jg_member_definition) {
            .name = jg_arena_strcpy(class_->allocator, name, MAX_NAME_LENGTH, class_->total_names_length),
            .next = class_->members,
            .getter = getter,
            .setter = setter,
        }
    );
    class_->member_count++;
    (*class_->total_member_count)++;
}

static jg_index build_class_index(
    jg_class* class_array,
    const char** index_pool,
    jg_member* member_pool,
    char* string_pool,
    jg_module_definition* module
);

jg_module* jg_module_build(jg_module_definition* module) {
    size_t class_count = module->class_count;
    size_t member_count = module->member_count;
    size_t names_length = module->names_length;

    jg_class* class_array = calloc(class_count, sizeof(jg_class));
    const char** index_pool = calloc(class_count + member_count, sizeof(char*));
    jg_member* member_pool = calloc(member_count, sizeof(jg_member));
    char* string_pool = calloc(names_length, sizeof(char));

    jg_module* new_module = malloc(sizeof(jg_module));
    
    *new_module = (jg_module) {
        .class_index = build_class_index(class_array, index_pool, member_pool, string_pool, module),
        .class_array = class_array,
        .index_pool = index_pool,
        .member_pool = member_pool,
        .string_pool = string_pool
    };

    jg_arena_free(module->allocator);

    return new_module;
}

void jg_module_free(jg_module* module) {
    free(module->class_array);
    free(module->index_pool);
    free(module->member_pool);
    free(module->string_pool);
    free(module);
}

const jg_class* jg_class_get(const jg_module* module, const char* name) {
    int class_id = jg_index_search(&module->class_index, name);
    if(class_id == -1) {
        return NULL;
    }

    return &(module->class_array[class_id]);
}

typedef struct jg_object_header_s {
    const jg_class* class_;
} jg_object_header_t;

void* jg_object_new(const jg_class* class_) {
    size_t total_size = sizeof(jg_object_header_t) + class_->size; // TODO(co): Align + class_->data_size;
    jg_object_header_t* header = calloc(1, total_size);
    header->class_ = class_;
    return header + 1;
}

void jg_object_free(void* object) {
    jg_object_header_t* header = object;
    free(header - 1);
}

jg_value jg_object_get(const void* object, const char* property_name) {
    const jg_object_header_t* header = (const jg_object_header_t*)object - 1;
    const jg_class* class_ = header->class_;
    while(class_) {
        int member_id = jg_index_search(&(class_->member_index), property_name);
        if(member_id >= 0) {
            return class_->member_array[member_id].getter(object);
        }
        class_ = class_->parent;
    }

    return jg_bool(false); // TODO(corentin@ki-dour.org): handle error
}

void jg_object_set(void* object, const char* property_name, const jg_value value) {
    const jg_object_header_t* header = (const jg_object_header_t*)object - 1;
    const jg_class* class_ = header->class_;
    while(class_) {
        int member_id = jg_index_search(&(class_->member_index), property_name);
        if(member_id >= 0) {
            class_->member_array[member_id].setter(object, value);
            return;
        }
        class_ = class_->parent;
    }
    JG_ASSERT(false);
}

// copies index keys to the given string pool. This is done after the indices are sorted,
// so the keys are in order for cache efficiency when searching.
static void index_pack(jg_index* index, char** string_pool) {
    for(size_t i = 0; i < index->count; ++i) {
        const char** key = &index->keys[i];
        size_t key_length = strlen(*key) + 1;
		*key = memcpy(*string_pool, *key, key_length);
        *string_pool += key_length;
    }
}

// create members for a class.
static void build_members(
    const char*** index_pool,
    jg_member** member_pool,
    char** string_pool,
    jg_class* class_,
    const jg_class_definition* class_definition
) {
    size_t member_count = class_definition->member_count;
    // allocate the needed key array in the pool.
    const char** member_keys = *index_pool;
    *index_pool += member_count;

    // initialize the member index keys
    const jg_member_definition* current_member = class_definition->members;
    for(size_t i = 0; i < member_count; ++i) {
        JG_ASSERT(current_member);
        member_keys[i] = current_member->name;
        current_member = current_member->next;
    }
    JG_ASSERT(!current_member);

    jg_index member_index = (jg_index) {
        .keys = member_keys,
        .count = member_count
    };

    jg_index_sort(&member_index);
    index_pack(&member_index, string_pool);

    // allocate needed members in the pool.
    jg_member* member_array = *member_pool;
    *member_pool += member_count;


    // create and store members, in sorted order.
    current_member = class_definition->members;
    for(size_t i = 0; i < member_count; ++i) {
        int member_id = jg_index_search(&member_index, current_member->name);
        JG_ASSERT(member_id >= 0 && (size_t)member_id < member_count);

        member_array[member_id] = (jg_member) {
            .getter = current_member->getter,
            .setter = current_member->setter
        };
        
        current_member = current_member->next;
    }
    JG_ASSERT(!current_member);

    class_->member_array = member_array;
    class_->member_index = member_index;
}

static void build_class(
    const jg_index* index,
    jg_class* class_array,
    const char*** index_pool,
    jg_member** member_pool,
    char** string_pool,
    jg_class_definition** sorted_definitions,
    size_t class_id
) {
    const jg_class_definition* class_definition = sorted_definitions[class_id];
    if(!class_definition) { return; } // class was already built

    const char* parent_name = class_definition->parent;
    const jg_class* parent = NULL;
    // if class has a parent, we build it first, so members and member keys are stored in the pools
    // in the depth-first hierarchy order.
    if(parent_name) {
        int parent_id = jg_index_search(index, parent_name);
        JG_ASSERT(parent_id >= 0 && (size_t)parent_id < index->count);
        build_class(index, class_array, index_pool, member_pool, string_pool, sorted_definitions, (size_t)parent_id);
        parent = &class_array[parent_id];
    }

    jg_class* new_class = &class_array[class_id];
    *new_class = (jg_class) {
        .parent = parent,
        .size = class_definition->size,
        .align = class_definition->align,
        .member_array = NULL
    };

    build_members(index_pool, member_pool, string_pool, new_class, class_definition);

    // set this so recursion don't build this class twice.
    sorted_definitions[class_id] = NULL;
}

static jg_index build_class_index(
    jg_class* class_array,
    const char** index_pool,
    jg_member* member_pool,
    char* string_pool,
    jg_module_definition* module
) {
    size_t class_count = module->class_count;
    // allocate needed keys in the index pool.
    const char** class_keys = index_pool;
    index_pool += class_count;

    jg_class_definition* current_class = module->class_definitions;
    for(size_t i = 0 ; i < class_count; ++i) {
        assert(current_class);
        class_keys[i] = current_class->name;
        current_class = current_class->next;
    }
    assert(!current_class);

    jg_index class_index = (jg_index) {
        .keys = class_keys,
        .count = class_count
    };
    jg_index_sort(&class_index);
    index_pack(&class_index, &string_pool);

    // build an array of class definition, sorted using the class_index.
    // TODO(corentin@ki-dour.org): if we can do this elsewhere than on the heap, it'd be nice.
    jg_class_definition** sorted_classes = calloc(class_count, sizeof(jg_class_definition*));
    current_class = module->class_definitions;
    for(size_t i = 0 ; i < class_count; ++i) {
        assert(current_class);
        int sorted_id = jg_index_search(&class_index, current_class->name);
        assert(sorted_id >= 0 && (size_t)sorted_id < class_count);
        sorted_classes[sorted_id] = current_class;
        current_class = current_class->next;
    }
    assert(!current_class);

    // we now build classes, so they can find their parent using the class_index.
    for(size_t class_id = 0; class_id < class_count; ++class_id) {
        build_class(
            &class_index,
            class_array,
            &index_pool,
            &member_pool,
            &string_pool,
            sorted_classes,
            class_id
        );
    }

    free(sorted_classes);
    return class_index;
}
