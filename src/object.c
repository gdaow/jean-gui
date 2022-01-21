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

#include "private/class.h"
#include "private/memory.h"
#include "private/misc.h"

void ulg_object_type(UlgClassDefinition* class_) {
    ulg_class_define(class_, "Object", 0);
}

typedef struct _UlgObjectHeader {
    const UlgClass* class_;
    char data[];
} UlgObjectHeader;

/*
static UlgObjectHeader* _get_header(void* object);
static const UlgObjectHeader* _get_header_const(const void* object);
*/

void* ulg_object_new(const UlgClass* class_) {
    size_t total_size = sizeof(UlgObjectHeader); // TODO: Align + class_->data_size;
    UlgObjectHeader* header = calloc(1, total_size);
    header->class_ = class_;
    return header->data;
}

void ulg_object_free(void* object) {
    (void)object;
    /*
    UlgObjectHeader* header = _get_header(object);
    const UlgObjectVT* vtable = ulg_object_vtable(object);
    if(vtable->cleanup) {
        vtable->cleanup(object);
    }

    free(header);
    */
}

UlgValue ulg_object_get(const void* object, const char* property_name) {
    (void)object;
    (void)property_name;
    /*
    const UlgObjectHeader* header = _get_header_const(object);
    const UlgClass* class_ = header->class_;
    const _UlgProperty* property = _get_property(class_, property_name);
    return property->getter(object);
    */
   return ulg_bool(true);
}

void ulg_object_set(void* object, const char* property_name, const UlgValue value) {
    (void)object;
    (void)property_name;
    (void)value;
    /*
    UlgObjectHeader* header = _get_header(object);
    const UlgClass* class_ = header->class_;
    const _UlgProperty* property = _get_property(class_, property_name);
    property->setter(object, value);
    */
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

/*
static UlgObjectHeader* _get_header(void* object) {
    return (UlgObjectHeader*)object - 1;
}

static const UlgObjectHeader* _get_header_const(const void* object) {
    return (const UlgObjectHeader*)object - 1;
}
*/
