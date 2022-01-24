/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <alloca.h>
#include <assert.h>
#include <malloc.h>
#include <stdalign.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <uiligi/object.h>

#include "private/memory.h"
#include "private/misc.h"

struct _UlgModuleDefinition {
    _Arena* allocator;
    UlgClassDefinition* class_definitions;
    size_t class_count;

    // total number of members to allocate for all classes of this module.
    size_t member_count;

    // used to accumulate the total length of class & member names, in order to allocate a big buffer for them all
    // when building modules.
    size_t names_length; 
};

UlgModuleDefinition* ulg_module_new() {
    _Arena* allocator = _arena_new(0);
    UlgModuleDefinition* module = _arena_alloc_init(allocator, &(UlgModuleDefinition) {
        .allocator = allocator,
        .class_definitions = NULL,
        .class_count = 0,
        .member_count = 0,
        .names_length = 0,
    });
    ulg_class_new(module, "Object", NULL, 0, 1);
    return module;
}

typedef struct __MemberDefinition _MemberDefinition;

struct _UlgClassDefinition {
    _Arena* allocator;
    const char* name;
    char* parent;
    size_t size;
    size_t align;
    UlgClassDefinition* next;
    _MemberDefinition* members;
    size_t member_count;
    size_t* total_member_count;
    size_t* total_names_length; // total size of classes and member names.
};

UlgClassDefinition* ulg_class_new(
    UlgModuleDefinition* module,
    const char* name,
    const char* parent,
    size_t size,
    size_t align
) {
    _Arena* allocator = module->allocator;
    size_t* names_length = &module->names_length;
    ++module->class_count;
    char* parent_copy = NULL;
    
    if(parent) {
        parent_copy = _arena_strcpy(allocator, parent, MAX_NAME_LENGTH, NULL);
    }

    UlgClassDefinition* new_class = _arena_alloc_init(
        allocator,
        &(UlgClassDefinition) {
            .allocator = module->allocator,
            .name = _arena_strcpy(allocator, name, MAX_NAME_LENGTH, names_length),
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

struct __MemberDefinition {
    const char* name;
    _MemberDefinition* next;
    UlgGetter getter;
    UlgSetter setter;
};

void ulg_class_add_property(UlgClassDefinition* class_, const char* name, UlgGetter getter, UlgSetter setter) {
    class_->members = _arena_alloc_init(
        class_->allocator,
        &(_MemberDefinition) {
            .name = _arena_strcpy(class_->allocator, name, MAX_NAME_LENGTH, class_->total_names_length),
            .next = class_->members,
            .getter = getter,
            .setter = setter,
        }
    );
    class_->member_count++;
    (*class_->total_member_count)++;
}

struct _UlgModule {
    _Index class_index;
    UlgClass* class_array;
    UlgMember* member_pool;
    const char** index_pool;
    char* string_pool;
};

struct _UlgClass {
    const UlgClass* parent;
    size_t size;
    size_t align;
    _Index member_index;
    UlgMember* member_array;
};

struct _UlgMember {
    UlgGetter getter;
    UlgSetter setter;
};

static _Index _build_class_index(
    UlgClass* class_array,
    const char** index_pool,
    UlgMember* member_pool,
    char* string_pool,
    UlgModuleDefinition* module
);

UlgModule* ulg_module_build(UlgModuleDefinition* module) {
    size_t class_count = module->class_count;
    size_t member_count = module->member_count;
    size_t names_length = module->names_length;

    UlgClass* class_array = calloc(class_count, sizeof(UlgClass));
    const char** index_pool = calloc(class_count + member_count, sizeof(char*));
    UlgMember* member_pool = calloc(member_count, sizeof(UlgMember));
    char* string_pool = calloc(names_length, sizeof(char));

    UlgModule* result = malloc_init(&(UlgModule) {
        .class_index = _build_class_index(class_array, index_pool, member_pool, string_pool, module),
        .class_array = class_array,
        .index_pool = index_pool,
        .member_pool = member_pool,
        .string_pool = string_pool
    });

    _arena_free(module->allocator);

    return result;
}

void ulg_module_free(UlgModule* module) {
    free(module->class_array);
    free(module->index_pool);
    free(module->member_pool);
    free(module->string_pool);
    free(module);
}

const UlgClass* ulg_class_get(const UlgModule* module, const char* name) {
    int class_id = _index_search(&module->class_index, name);
    if(class_id == -1) {
        return NULL;
    }

    return &(module->class_array[class_id]);
}

typedef struct _UlgObjectHeader {
    const UlgClass* class_;
} UlgObjectHeader;

void* ulg_object_new(const UlgClass* class_) {
    size_t total_size = sizeof(UlgObjectHeader) + class_->size; // TODO: Align + class_->data_size;
    UlgObjectHeader* header = calloc(1, total_size);
    header->class_ = class_;
    return header + 1;
}

void ulg_object_free(void* object) {
    UlgObjectHeader* header = object;
    free(header - 1);
}

UlgValue ulg_object_get(const void* object, const char* property_name) {
    const UlgObjectHeader* header = (const UlgObjectHeader*)object - 1;
    const UlgClass* class_ = header->class_;
    while(class_) {
        int member_id = _index_search(&(class_->member_index), property_name);
        if(member_id >= 0)
            return class_->member_array[member_id].getter(object);
        class_ = class_->parent;
    }

    return ulg_bool(false); // TODO : handle error
}

void ulg_object_set(void* object, const char* property_name, const UlgValue value) {
    const UlgObjectHeader* header = (const UlgObjectHeader*)object - 1;
    const UlgClass* class_ = header->class_;
    while(class_) {
        int member_id = _index_search(&(class_->member_index), property_name);
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
static void _pack_index(_Index* index, char** string_pool) {
    for(size_t i = 0; i < index->count; ++i) {
        const char** key = &index->keys[i];
        size_t key_length = strlen(*key) + 1;
		*key = memcpy(*string_pool, *key, key_length);
        *string_pool += key_length;
    }
}

// create members for a class.
static void _build_members(
    const char*** index_pool,
    UlgMember** member_pool,
    char** string_pool,
    UlgClass* class_,
    const UlgClassDefinition* class_definition
) {
    size_t member_count = class_definition->member_count;
    // allocate the needed key array in the pool.
    const char** member_keys = *index_pool;
    *index_pool += member_count;

    // initialize the member index keys
    const _MemberDefinition* current_member = class_definition->members;
    for(size_t i = 0; i < member_count; ++i) {
        assert(current_member);
        member_keys[i] = current_member->name;
        current_member = current_member->next;
    }
    assert(!current_member);

    _Index member_index = (_Index) {
        .keys = member_keys,
        .count = member_count
    };

    _index_sort(&member_index);
    _pack_index(&member_index, string_pool);

    // allocate needed members in the pool.
    UlgMember* member_array = *member_pool;
    *member_pool += member_count;


    // create and store members, in sorted order.
    current_member = class_definition->members;
    for(size_t i = 0; i < member_count; ++i) {
        int member_id = _index_search(&member_index, current_member->name);
        assert(member_id >= 0 && (size_t)member_id < member_count);

        member_array[member_id] = (UlgMember) {
            .getter = current_member->getter,
            .setter = current_member->setter
        };
        
        current_member = current_member->next;
    }
    assert(!current_member);

    class_->member_array = member_array;
    class_->member_index = member_index;
}

static void _build_class(
    const _Index* index,
    UlgClass* class_array,
    const char*** index_pool,
    UlgMember** member_pool,
    char** string_pool,
    UlgClassDefinition** sorted_definitions,
    size_t class_id
) {
    const UlgClassDefinition* class_definition = sorted_definitions[class_id];
    if(!class_definition) // already built
        return;

    const char* parent_name = class_definition->parent;
    const UlgClass* parent = NULL;
    // if class has a parent, we build it first, so members and member keys are stored in the pools
    // in the depth-first hierarchy order.
    if(parent_name) {
        int parent_id = _index_search(index, parent_name);
        assert(parent_id >= 0 && (size_t)parent_id < index->count);
        _build_class(index, class_array, index_pool, member_pool, string_pool, sorted_definitions, (size_t)parent_id);
        parent = &class_array[parent_id];
    }

    UlgClass* new_class = &class_array[class_id];
    *new_class = (UlgClass) {
        .parent = parent,
        .size = class_definition->size,
        .align = class_definition->align,
        .member_array = NULL
    };

    _build_members(index_pool, member_pool, string_pool, new_class, class_definition);

    // set this so recursion don't build this class twice.
    sorted_definitions[class_id] = NULL;
}

static _Index _build_class_index(
    UlgClass* class_array,
    const char** index_pool,
    UlgMember* member_pool,
    char* string_pool,
    UlgModuleDefinition* module
) {
    size_t class_count = module->class_count;
    // allocate needed keys in the index pool.
    const char** class_keys = index_pool;
    index_pool += class_count;

    UlgClassDefinition* current_class = module->class_definitions;
    for(size_t i = 0 ; i < class_count; ++i) {
        assert(current_class);
        class_keys[i] = current_class->name;
        current_class = current_class->next;
    }
    assert(!current_class);

    _Index class_index = (_Index) {
        .keys = class_keys,
        .count = class_count
    };
    _index_sort(&class_index);
    _pack_index(&class_index, &string_pool);

    // build an array of class definition on the stack, sorted using the class_index.
    UlgClassDefinition** sorted_classes = alloca(sizeof(UlgClassDefinition) * class_count);
    current_class = module->class_definitions;
    for(size_t i = 0 ; i < class_count; ++i) {
        assert(current_class);
        int sorted_id = _index_search(&class_index, current_class->name);
        assert(sorted_id >= 0 && (size_t)sorted_id < class_count);
        sorted_classes[sorted_id] = current_class;
        current_class = current_class->next;
    }
    assert(!current_class);

    // we now build classes, so they can find their parent using the class_index.
    for(size_t class_id = 0; class_id < class_count; ++class_id)
        _build_class(
            &class_index,
            class_array,
            &index_pool,
            &member_pool,
            &string_pool,
            sorted_classes,
            class_id
        );

    return class_index;
}
