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

#include <uiligi/object.h>

#include "private/memory.h"
#include "private/misc.h"

struct ulg_module_definition_s {
    ulg_arena_t* allocator;
    ulg_class_definition_t* class_definitions;
    size_t class_count;

    // total number of members to allocate for all classes of this module.
    size_t member_count;

    // used to accumulate the total length of class & member names, in order to allocate a big buffer for them all
    // when building modules.
    size_t names_length; 
};

typedef struct ulg_member_definition_t ulg_member_definition_t;

struct ulg_class_definition_s {
    ulg_arena_t* allocator;
    const char* name;
    char* parent;
    size_t size;
    size_t align;
    ulg_class_definition_t* next;
    ulg_member_definition_t* members;
    size_t member_count;
    size_t* total_member_count;
    size_t* total_names_length; // total size of classes and member names.
};

struct ulg_module_s {
    ulg_index_t class_index;
    ulg_class_t* class_array;
    ulg_member_t* member_pool;
    const char** index_pool;
    char* string_pool;
};

struct ulg_class_s {
    ulg_index_t member_index;
    const ulg_class_t* parent;
    ulg_member_t* member_array;
    size_t size;
    size_t align;
};

struct ulg_member_s {
    ulg_getter_t getter;
    ulg_setter_t setter;
};

ulg_module_definition_t* ulg_module_new() {
    ulg_arena_t* allocator = ulg_arena_new(0);
    ulg_module_definition_t* module = ULG_ARENA_ALLOC_INIT(
        allocator,
        ulg_module_definition_t,
        &(ulg_module_definition_t) {
            .allocator = allocator,
            .class_definitions = NULL,
            .class_count = 0,
            .member_count = 0,
            .names_length = 0,
        }
    );
    ulg_class_new(module, "Object", NULL, 0, 1);
    return module;
}

ulg_class_definition_t* ulg_class_new(
    ulg_module_definition_t* module,
    const char* name,
    const char* parent,
    size_t size,
    size_t align
) {
    ulg_arena_t* allocator = module->allocator;
    size_t* names_length = &module->names_length;
    ++module->class_count;
    char* parent_copy = NULL;
    
    if(parent) {
        parent_copy = ulg_arena_strcpy(allocator, parent, MAX_NAME_LENGTH, NULL);
    }

    ulg_class_definition_t* new_class = ULG_ARENA_ALLOC_INIT(
        allocator,
        ulg_class_definition_t,
        &(ulg_class_definition_t) {
            .allocator = module->allocator,
            .name = ulg_arena_strcpy(allocator, name, MAX_NAME_LENGTH, names_length),
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

struct ulg_member_definition_t {
    const char* name;
    ulg_member_definition_t* next;
    ulg_getter_t getter;
    ulg_setter_t setter;
};

void ulg_class_add_property(ulg_class_definition_t* class_, const char* name, ulg_getter_t getter, ulg_setter_t setter) {
    class_->members = ULG_ARENA_ALLOC_INIT(
        class_->allocator,
        ulg_member_definition_t,
        &(ulg_member_definition_t) {
            .name = ulg_arena_strcpy(class_->allocator, name, MAX_NAME_LENGTH, class_->total_names_length),
            .next = class_->members,
            .getter = getter,
            .setter = setter,
        }
    );
    class_->member_count++;
    (*class_->total_member_count)++;
}

static ulg_index_t build_class_index(
    ulg_class_t* class_array,
    const char** index_pool,
    ulg_member_t* member_pool,
    char* string_pool,
    ulg_module_definition_t* module
);

ulg_module_t* ulg_module_build(ulg_module_definition_t* module) {
    size_t class_count = module->class_count;
    size_t member_count = module->member_count;
    size_t names_length = module->names_length;

    ulg_class_t* class_array = calloc(class_count, sizeof(ulg_class_t));
    const char** index_pool = calloc(class_count + member_count, sizeof(char*));
    ulg_member_t* member_pool = calloc(member_count, sizeof(ulg_member_t));
    char* string_pool = calloc(names_length, sizeof(char));

    ulg_module_t* result = ULG_MALLOC_INIT(&(ulg_module_t) {
        .class_index = build_class_index(class_array, index_pool, member_pool, string_pool, module),
        .class_array = class_array,
        .index_pool = index_pool,
        .member_pool = member_pool,
        .string_pool = string_pool
    });

    ulg_arena_free(module->allocator);

    return result;
}

void ulg_module_free(ulg_module_t* module) {
    free(module->class_array);
    free(module->index_pool);
    free(module->member_pool);
    free(module->string_pool);
    free(module);
}

const ulg_class_t* ulg_class_get(const ulg_module_t* module, const char* name) {
    int class_id = ulg_index_search(&module->class_index, name);
    if(class_id == -1) {
        return NULL;
    }

    return &(module->class_array[class_id]);
}

typedef struct ulg_object_header_s {
    const ulg_class_t* class_;
} ulg_object_header_t;

void* ulg_object_new(const ulg_class_t* class_) {
    size_t total_size = sizeof(ulg_object_header_t) + class_->size; // TODO(co): Align + class_->data_size;
    ulg_object_header_t* header = calloc(1, total_size);
    header->class_ = class_;
    return header + 1;
}

void ulg_object_free(void* object) {
    ulg_object_header_t* header = object;
    free(header - 1);
}

ulg_value_t ulg_object_get(const void* object, const char* property_name) {
    const ulg_object_header_t* header = (const ulg_object_header_t*)object - 1;
    const ulg_class_t* class_ = header->class_;
    while(class_) {
        int member_id = ulg_index_search(&(class_->member_index), property_name);
        if(member_id >= 0) {
            return class_->member_array[member_id].getter(object);
        }
        class_ = class_->parent;
    }

    return ulg_bool(false); // TODO(corentin@ki-dour.org): handle error
}

void ulg_object_set(void* object, const char* property_name, const ulg_value_t value) {
    const ulg_object_header_t* header = (const ulg_object_header_t*)object - 1;
    const ulg_class_t* class_ = header->class_;
    while(class_) {
        int member_id = ulg_index_search(&(class_->member_index), property_name);
        if(member_id >= 0) {
            class_->member_array[member_id].setter(object, value);
            return;
        }
        class_ = class_->parent;
    }
    assert(false);
}

// copies index keys to the given string pool. This is done after the indices are sorted,
// so the keys are in order for cache efficiency when searching.
static void index_pack(ulg_index_t* index, char** string_pool) {
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
    ulg_member_t** member_pool,
    char** string_pool,
    ulg_class_t* class_,
    const ulg_class_definition_t* class_definition
) {
    size_t member_count = class_definition->member_count;
    // allocate the needed key array in the pool.
    const char** member_keys = *index_pool;
    *index_pool += member_count;

    // initialize the member index keys
    const ulg_member_definition_t* current_member = class_definition->members;
    for(size_t i = 0; i < member_count; ++i) {
        assert(current_member);
        member_keys[i] = current_member->name;
        current_member = current_member->next;
    }
    assert(!current_member);

    ulg_index_t member_index = (ulg_index_t) {
        .keys = member_keys,
        .count = member_count
    };

    ulg_index_sort(&member_index);
    index_pack(&member_index, string_pool);

    // allocate needed members in the pool.
    ulg_member_t* member_array = *member_pool;
    *member_pool += member_count;


    // create and store members, in sorted order.
    current_member = class_definition->members;
    for(size_t i = 0; i < member_count; ++i) {
        int member_id = ulg_index_search(&member_index, current_member->name);
        assert(member_id >= 0 && (size_t)member_id < member_count);

        member_array[member_id] = (ulg_member_t) {
            .getter = current_member->getter,
            .setter = current_member->setter
        };
        
        current_member = current_member->next;
    }
    assert(!current_member);

    class_->member_array = member_array;
    class_->member_index = member_index;
}

static void build_class(
    const ulg_index_t* index,
    ulg_class_t* class_array,
    const char*** index_pool,
    ulg_member_t** member_pool,
    char** string_pool,
    ulg_class_definition_t** sorted_definitions,
    size_t class_id
) {
    const ulg_class_definition_t* class_definition = sorted_definitions[class_id];
    if(!class_definition) { return; } // class was already built

    const char* parent_name = class_definition->parent;
    const ulg_class_t* parent = NULL;
    // if class has a parent, we build it first, so members and member keys are stored in the pools
    // in the depth-first hierarchy order.
    if(parent_name) {
        int parent_id = ulg_index_search(index, parent_name);
        assert(parent_id >= 0 && (size_t)parent_id < index->count);
        build_class(index, class_array, index_pool, member_pool, string_pool, sorted_definitions, (size_t)parent_id);
        parent = &class_array[parent_id];
    }

    ulg_class_t* new_class = &class_array[class_id];
    *new_class = (ulg_class_t) {
        .parent = parent,
        .size = class_definition->size,
        .align = class_definition->align,
        .member_array = NULL
    };

    build_members(index_pool, member_pool, string_pool, new_class, class_definition);

    // set this so recursion don't build this class twice.
    sorted_definitions[class_id] = NULL;
}

static ulg_index_t build_class_index(
    ulg_class_t* class_array,
    const char** index_pool,
    ulg_member_t* member_pool,
    char* string_pool,
    ulg_module_definition_t* module
) {
    size_t class_count = module->class_count;
    // allocate needed keys in the index pool.
    const char** class_keys = index_pool;
    index_pool += class_count;

    ulg_class_definition_t* current_class = module->class_definitions;
    for(size_t i = 0 ; i < class_count; ++i) {
        assert(current_class);
        class_keys[i] = current_class->name;
        current_class = current_class->next;
    }
    assert(!current_class);

    ulg_index_t class_index = (ulg_index_t) {
        .keys = class_keys,
        .count = class_count
    };
    ulg_index_sort(&class_index);
    index_pack(&class_index, &string_pool);

    // build an array of class definition, sorted using the class_index.
    // TODO(corentin@ki-dour.org): if we can do this elsewhere than on the heap, it'd be nice.
    ulg_class_definition_t** sorted_classes = calloc(class_count, sizeof(ulg_class_definition_t*));
    current_class = module->class_definitions;
    for(size_t i = 0 ; i < class_count; ++i) {
        assert(current_class);
        int sorted_id = ulg_index_search(&class_index, current_class->name);
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
