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

#include <uiligi/object.h>

struct _UlgModule {
    int dummy;
};

struct _UlgClass {
    uintptr_t id;
    const char* name;
    size_t data_size;
    struct hashmap* properties;
    const UlgClass* parent;
    size_t vtable_size; // This is here for checks only, could be removed.
    char vtable[];
};

typedef struct {
    const UlgClass* class_;
} _ClassItem;

typedef struct _UlgClassFactory {
    UlgModule* module;
    uintptr_t class_id;
} UlgClassFactory;


UlgModule* ulg_module_new() {
    UlgModule* module = malloc(sizeof(UlgModule));
    // Class data is owned by the name index.
    ulg_class_register(module, ulg_object_type);

    return module;
}

void ulg_module_free(UlgModule* module) {
    free(module);
}

void ulg_class_register(UlgModule* module, UlgClassDefinition definition) {
    (void)module;
    (void)definition;
    /*
    const UlgClass* new_class = definition(
        &(UlgClassFactory) {
            .class_id = (uintptr_t)definition, // To pass the id down to ulg_class_declare.
            .module = module
        }
    );

    void* replaced_item = NULL;
    replaced_item = hashmap_set(
        module->name_index,
        &(_ClassItem) { .class_ = new_class }
    );
    assert(!replaced_item);

    replaced_item = hashmap_set(
        module->id_index,
        &(_ClassItem) { .class_ = new_class }
    );
    assert(!replaced_item);
    */
}

void ulg_class_declare(
    UlgClassFactory* factory,
    const char* name,
    UlgClassDefinition parent_definition
) {
    (void)factory;
    (void)name;
    (void)parent_definition;
    /*
    const UlgClass* parent = NULL;
    // We juste use an obfuscated type to prevent direct calls to ulg_class_declare
    // but allow them only in UlgClassDefinition callbacks.
    UlgModule* module = factory->module;
    if(parent_definition) {
        parent = ulg_class_get(module, parent_definition);
    }
    UlgClass* class_ =  _class_new(name, data_size, parent, vtable_size);
    class_->id = factory->class_id;
    return class_;
    */
}

void* ulg_class_create_vtable(UlgClassFactory* factory, size_t vtable_size, size_t vtable_align) {
    (void)factory;
    (void)vtable_size;
    (void)vtable_align;
    return NULL;
}

typedef struct {
    const char* name;
    UlgGetter getter;
    UlgSetter setter;
} _UlgProperty;

void ulg_class_add_property(UlgClassFactory* factory, const char* name, UlgGetter getter, UlgSetter setter) {
    (void)factory;
    (void)name;
    (void)getter;
    (void)setter;
}

const UlgClass* ulg_class_get(UlgModule* module, UlgClassDefinition definition) {
    (void)module;
    (void)definition;
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

void ulg_object_type(UlgClassFactory* factory) {
    ulg_class_declare(factory, "Object", 0);
    memset(
        ulg_class_create_vtable(factory, sizeof(UlgObjectVT), alignof(UlgObjectVT)),
        0,
        sizeof(UlgObjectVT)
    );
}

typedef struct _UlgObjectHeader {
    const UlgClass* class_;
    char data[];
} UlgObjectHeader;

static UlgObjectHeader* _get_header(void* object);
static const UlgObjectHeader* _get_header_const(const void* object);

void* ulg_object_new(const UlgClass* class_) {
    size_t total_size = sizeof(UlgObjectHeader) + class_->data_size;
    UlgObjectHeader* header = calloc(1, total_size);
    header->class_ = class_;
    const UlgObjectVT* vtable = ulg_object_vtable(header->data);
    if(vtable->init) {
        vtable->init(header->data);
    }
    return header->data;
}

void ulg_object_free(void* object) {
    UlgObjectHeader* header = _get_header(object);
    const UlgObjectVT* vtable = ulg_object_vtable(object);
    if(vtable->cleanup) {
        vtable->cleanup(object);
    }

    free(header);
}

const void* ulg_object_vtable(const void* object) {
    const UlgObjectHeader* header = _get_header_const(object);
    return header->class_->vtable;
}

static const _UlgProperty* _get_property(const UlgClass* class_, const char* name);

UlgValue ulg_object_get(const void* object, const char* property_name) {
    const UlgObjectHeader* header = _get_header_const(object);
    const UlgClass* class_ = header->class_;
    const _UlgProperty* property = _get_property(class_, property_name);
    return property->getter(object);
}

void ulg_object_set(void* object, const char* property_name, const UlgValue value) {
    UlgObjectHeader* header = _get_header(object);
    const UlgClass* class_ = header->class_;
    const _UlgProperty* property = _get_property(class_, property_name);
    property->setter(object, value);
}

static const _UlgProperty* _get_property(const UlgClass* class_, const char* name) {
    (void)class_;
    (void)name;
    /*
    _UlgProperty* result = hashmap_get(
        class_->properties,
        &(_UlgProperty){ .name = name }
    );
    assert(result); // TODO: Handle error.
    return result;
    */
   return NULL;
}

/*
static UlgClass* _class_new(
    const char* name,
    size_t data_size,
    const UlgClass* parent,
    size_t vtable_size
) {
    (void)name;
    (void)data_size;
    (void)parent;
    (void)vtable_size;
    // vtable size can not be less than the parent one.
    if(vtable_size == 0 && parent) {
        vtable_size = parent->vtable_size;
    }
    else {
        assert(!parent || parent->vtable_size <= vtable_size);
    }

    UlgClass* class_ = memcpy(
        malloc(sizeof(UlgClass) + vtable_size),
        &(UlgClass) {
            .name = name,
            .data_size = data_size,
            .parent = parent,
            .vtable_size = vtable_size
        },
        sizeof(UlgClass)
    );

    class_->properties = hashmap_new(
        sizeof(_UlgProperty),
        0, 0, 0,
        &_property_hash,
        &_property_compare,
        NULL,
        NULL
    );

    if(parent) {
        memcpy(class_->vtable, parent->vtable, parent->vtable_size);
        hashmap_scan(parent->properties, _copy_parent_properties, class_->properties);
    }

    return class_;
   return NULL;
}
*/

static UlgObjectHeader* _get_header(void* object) {
    return (UlgObjectHeader*)object - 1;
}

static const UlgObjectHeader* _get_header_const(const void* object) {
    return (const UlgObjectHeader*)object - 1;
}
