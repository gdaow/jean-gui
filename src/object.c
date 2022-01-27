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

#include "jg/value.h"
#include "private/memory.h"
#include "private/misc.h"


static const size_t JG_MAX_IDENTIFIER_LENGTH = 256;

struct jg_module_definition_s {
    jg_arena* arena;
    jg_class_definition* first_class;
};

typedef struct jg_member_definition_s jg_member_definition;

struct jg_class_definition_s {
    jg_arena* arena;
    const char* id;
    char* parent;
    size_t size;
    size_t align;
    jg_class_definition* next_class;
    jg_member_definition* first_member;
};

typedef enum {
    JG_MEMBER_PROPERTY,
    JG_MEMBER_METHOD
} jg_member_type;

typedef union {
    struct {
        jg_getter_t getter;
        jg_setter_t setter;
    } property;
    jg_method method;
} jg_member_data;

struct jg_member_definition_s {
    const char* id;
    jg_member_type type;
    jg_member_data data;
    jg_member_definition* next_member;
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
    jg_member_type type;
    jg_member_data data;
};

typedef struct jg_object_header_s {
    const jg_class* class_;
} jg_object_header;

jg_module_definition* jg_module_new() {
    jg_arena* arena = jg_arena_new(0);
    jg_module_definition* new_module = jg_arena_alloc(
        arena,
        sizeof(jg_module_definition),
        alignof(jg_module_definition)
    );

    *new_module = (jg_module_definition) { 0 };
    new_module->arena = arena;
    jg_class_new(new_module, "Object", NULL, 0, 1);

    return new_module;
}

static char* copy_identifier(jg_arena* arena, const char* value) {
    return jg_arena_strcpy(arena, value, JG_MAX_IDENTIFIER_LENGTH);
}

jg_class_definition* jg_class_new(
    jg_module_definition* module,
    const char* id,
    const char* parent,
    size_t size,
    size_t align
) {
    jg_arena* arena = module->arena;
    char* parent_copy = NULL;
    
    if(parent) {
        parent_copy = copy_identifier(arena, parent);
    }

    jg_class_definition* new_class = jg_arena_alloc(
        arena,
        sizeof(jg_class_definition),
        alignof(jg_class_definition)
    );
    
    *new_class = (jg_class_definition) {
        .arena = module->arena,
        .id = copy_identifier(arena, id),
        .parent = parent_copy,
        .size = size,
        .align = align,
        .next_class = module->first_class,
        .first_member = 0
    };

    module->first_class = new_class;

    return new_class;
}


static void add_member(
    jg_class_definition* class_,
    const char* id,
    const jg_member_definition* member
) {
    jg_member_definition* new_member = jg_arena_alloc(
        class_->arena,
        sizeof(jg_member_definition),
        alignof(jg_member_definition)
    );

    *new_member = *member;
    new_member->id = copy_identifier(class_->arena, id);
    new_member->next_member = class_->first_member;
    class_->first_member = new_member;
}

void jg_class_add_property(jg_class_definition* class_, const char* id, jg_getter_t getter, jg_setter_t setter) {
    add_member(class_, id, &(jg_member_definition) {
        .type = JG_MEMBER_PROPERTY,
        .data.property.getter = getter,
        .data.property.setter = setter
    });
}

void jg_class_add_method(
    jg_class_definition* class_,
    const char* id,
    jg_method method
) {
    add_member(class_, id, &(jg_member_definition) {
        .type = JG_MEMBER_METHOD,
        .data.method = method
    });
}

// build an index given a class or member definition
static jg_index build_index(
    void* definition,
    const char* (*get_id)(void** next_definition),
    const char*** index_pool,
    char** string_pool
 ) {
    const char** keys = *index_pool;

    while(definition) {
        // allocate a key in the index pool
        *((*index_pool)++) = get_id(&definition);
    }
    JG_ASSERT(*index_pool >= keys);
    size_t count = (size_t)(*index_pool - keys); // total of allocated slots
 
    jg_index result = (jg_index) {
        .keys = keys,
        .count = count
    };

    jg_index_sort(&result);

    // pack ids consecutively in the module's string pool.
    for(size_t i = 0; i < count; ++i) {
        const char** key = &keys[i];
        size_t key_length = strlen(*key) + 1;
		*key = memcpy(*string_pool, *key, key_length);
        *string_pool += key_length;
    }
    
    return result;
}

static const char* get_member_id(void** current_definition) {
    jg_member_definition* member_definition = *current_definition;
    const char* id = member_definition->id;
    *current_definition = member_definition->next_member;
    return id;
}

// create members for a class.
static void build_members(
    const char*** index_pool,
    jg_member** member_pool,
    char** string_pool,
    jg_class* class_,
    const jg_class_definition* class_definition
) {
    jg_index member_index = build_index(
        class_definition->first_member,
        get_member_id,
        index_pool,
        string_pool
    );
    // allocate needed members in the pool.
    jg_member* member_array = *member_pool;
    *member_pool += member_index.count;

    // create and store members, in sorted order.
    jg_member_definition* current_member = class_definition->first_member;
    while(current_member) {
        int member_id = jg_index_search(&member_index, current_member->id);
        JG_ASSERT(member_id >= 0 && (size_t)member_id < member_index.count);

        member_array[member_id] = (jg_member) {
            .type = current_member->type,
            .data = current_member->data
        };

        current_member = current_member->next_member;
    }

    class_->member_array = member_array;
    class_->member_index = member_index;
}

static const char* get_class_id(void** current_definition) {
    jg_class_definition* class_definition = *current_definition;
    const char* id = class_definition->id;
    *current_definition = class_definition->next_class;
    return id;
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

    const char* parent_string_id = class_definition->parent;
    const jg_class* parent = NULL;
    // if class has a parent, we build it first, so members and member keys are stored in the pools
    // in the depth-first hierarchy order.
    if(parent_string_id) {
        int parent_id = jg_index_search(index, parent_string_id);
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

static void get_pools_size(
    jg_module_definition* module,
    size_t* class_count,
    size_t* member_count,
    size_t* id_length
) {
    jg_class_definition* class_ = module->first_class;
    while(class_) {
        ++*class_count;
        *id_length += strlen(class_->id) + 1; // for terminal \0
        jg_member_definition* member = class_->first_member;
        while(member) {
            ++*member_count;
            *id_length += strlen(member->id) + 1; // for terminal \0
            member = member->next_member;
        }
        class_ = class_->next_class;
    }
}

jg_index build_class_index(
    jg_module_definition* module,
    jg_class* class_array,
    const char**index_pool,
    jg_member* member_pool,
    char* string_pool
) {

    jg_index class_index = build_index(
        module->first_class,
        get_class_id,
        &index_pool,
        &string_pool
    );
    size_t class_count = class_index.count;

    // allocate needed keys in the index pool.
    // build an array of class definition, sorted using the class_index.
    // TODO(corentin@ki-dour.org): if we can do this elsewhere than on the heap, it'd be nice.
    jg_class_definition** sorted_classes = calloc(class_count, sizeof(jg_class_definition*));
    jg_class_definition* current_class = module->first_class;
    while(current_class) {
        int sorted_id = jg_index_search(&class_index, current_class->id);
        JG_ASSERT(sorted_id >= 0 && (size_t)sorted_id < class_count);
        sorted_classes[sorted_id] = current_class;
        current_class = current_class->next_class;
    }

    for(size_t i = 0; i < class_index.count; ++i) {
        build_class(&class_index, class_array, &index_pool, &member_pool, &string_pool, sorted_classes, i);
    }

    free(sorted_classes);

    return class_index;
}

jg_module* jg_module_build(jg_module_definition* module) {
    size_t class_count = 0;
    size_t member_count = 0;
    size_t names_length = 0;

    get_pools_size(module, &class_count, &member_count, &names_length);

    jg_class* class_array = calloc(class_count, sizeof(jg_class));
    const char** index_pool = calloc(class_count + member_count, sizeof(char*));
    jg_member* member_pool = calloc(member_count, sizeof(jg_member));
    char* string_pool = calloc(names_length, sizeof(char));
    jg_index class_index = build_class_index(module, class_array, index_pool, member_pool, string_pool);

    jg_arena_free(module->arena);

    return memcpy(
        malloc(sizeof(jg_module)),
        &(jg_module) {
            .class_index = class_index,
            .class_array = class_array,
            .index_pool = index_pool,
            .member_pool = member_pool,
            .string_pool = string_pool
        },
        sizeof(jg_module)
    );
}

void jg_module_free(jg_module* module) {
    free(module->class_array);
    free(module->index_pool);
    free(module->member_pool);
    free(module->string_pool);
    free(module);
}

const jg_class* jg_class_get(const jg_module* module, const char* id) {
    int class_id = jg_index_search(&module->class_index, id);
    if(class_id == -1) {
        return NULL;
    }

    return &(module->class_array[class_id]);
}

void* jg_object_new(const jg_class* class_) {
    size_t total_size = sizeof(jg_object_header) + class_->size; // TODO(co): Align + class_->data_size;
    jg_object_header* header = calloc(1, total_size);
    header->class_ = class_;
    return header + 1;
}

void jg_object_free(void* object) {
    jg_object_header* header = object;
    free(header - 1);
}

static jg_member* get_member(const void* object, const char* id, jg_member_type type) {
    const jg_object_header* header = (const jg_object_header*)object - 1;
    const jg_class* class_ = header->class_;
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

jg_value jg_object_get(const void* object, const char* property_id) {
    jg_member* member = get_member(object, property_id, JG_MEMBER_PROPERTY);
    return member->data.property.getter(object);
}

void jg_object_set(void* object, const char* property_id, const jg_value value) {
    jg_member* member = get_member(object, property_id, JG_MEMBER_PROPERTY);
    member->data.property.setter(object, value);
}

jg_value jg_object_call(
    const void* object,
    const char* method_id,
    jg_value* arguments,
    size_t argument_count
){
    jg_member* member = get_member(object, method_id, JG_MEMBER_METHOD);
    return member->data.method(arguments, argument_count);
}
