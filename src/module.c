/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <assert.h>
#include <stdalign.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <uiligi/module.h>
#include <uiligi/object.h>

#include "private/memory.h"
#include "private/misc.h"
#include "private/class.h"

typedef struct {
    UlgClass* class_;
} _NameIndex;

struct _UlgModule {
    size_t nb_classes;
    _NameIndex* name_index;
};

struct _UlgModuleDefinition {
    UlgArena* allocator;
    size_t nb_classes;
    size_t class_names_length;
    size_t member_names_length;
    UlgClassDefinition* classes;
};

UlgModuleDefinition* ulg_module_new() {
    UlgArena* arena = ulg_arena_new(0);
    UlgModuleDefinition* module = ulg_arena_alloc_struct(arena, &(UlgModuleDefinition) {
        .allocator = arena
    });
    // Class data is owned by the name index.
    ulg_class_register(module, ulg_object_type);

    return module;
}

static void _get_class_size(UlgClassDefinition* class_, size_t* size) {
    ulg_buffer_calloc(0, size, UlgMember, class_->nb_members);
    ulg_buffer_calloc(0, size, char, class_->names_length) ;
}

static void _get_module_size(UlgModuleDefinition* module, size_t* size) {
    ulg_buffer_alloc(0, size, &(UlgModule) {
        .name_index = NULL
    });
    ulg_buffer_calloc(0, size, _NameIndex, module->nb_classes);
    ulg_buffer_calloc(0, size, char, module->class_names_length);

    UlgClassDefinition* class_ = module->classes;
    while(class_) {
        _get_class_size(class_, size);
        class_ = class_->next;
    }
}

UlgModule* ulg_module_build(UlgModuleDefinition* module) {
    size_t module_size = 0;
    _get_module_size(module, &module_size);
    void* buffer = malloc(module_size);
    size_t offset = 0;
    UlgModule* result = ulg_buffer_alloc(buffer, &offset, &(UlgModule){
        .nb_classes = module->nb_classes,
        .name_index = ulg_buffer_calloc(buffer, &offset, _NameIndex, module->nb_classes)
    });

    //char* name_buffer = ulg_buffer_calloc(buffer, &offset, char, module->member_names_length);

    UlgClassDefinition* class_ = module->classes;
    _NameIndex* name_index = result->name_index;
    while(class_) {
        name_index->class_ = ulg_buffer_alloc(buffer, &offset, &(UlgClass) {
            .id = class_->id,
            .parent_id = class_->parent_id,
            .name = class_->name,
        });
        ++name_index;
    }

    return NULL;
}

void ulg_module_free(UlgModule* module) {
    free(module);
}

void ulg_class_register(UlgModuleDefinition* module, UlgClassDeclaration class_) {
    module->classes = ulg_arena_alloc_struct(
        module->allocator,
        &(UlgClassDefinition) {
            .allocator = module->allocator,
            .id = (uintptr_t)class_,
            .next = module->classes,
            .properties = NULL
        }
    );
    class_(module->classes);
}

const UlgClass* ulg_class_get(const UlgModule* module, UlgClassDeclaration class_) {
    (void)module;
    (void)class_;
    /*
    uintptr_t class_id = (uintptr_t)definition; // TODO: will be anoying when wrapping to other languages.
    _ClassItem* item = hashmap_get(
        module->id_index,
        &(_ClassItem) {
            .class_ = &(UlgClass) { .id = class_id }
        }
    );

    if(!item) return NULL;

    return item->class_;
    */
   return NULL;
}

const UlgClass* ulg_class_get_by_name(UlgModule* module, const char* name) {
    (void)module;
    (void)name;
    return NULL;
    /*
    _ClassItem* item = hashmap_get(
        module->name_index,
        &(_ClassItem) {
            .class_ = &(UlgClass) { .name = name }
        }
    );
    if(!item) return NULL;
    return item->class_;
    */
}
