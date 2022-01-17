#pragma once
#include <stddef.h>

#include <uiligi/class.h>

#include "utils/hashmap.h"

typedef struct _UlgClass UlgClass;

typedef struct _UlgProperty {
    const char* name;
    UlgGetter getter;
    UlgSetter setter;
} UlgProperty;

struct _UlgClass {
    uint64_t id;
    const char* name;
    size_t data_size;
    struct hashmap* properties;
    const UlgClass* parent;
    size_t vtable_size; // This is here for checks only, could be removed.
    char vtable[];
};

UlgClass* _ulg_class_new(
    const char* name,
    size_t data_size,
    const UlgClass* parent,
    size_t vtable_size
);

void _ulg_class_free(UlgClass* class_);

const UlgProperty* _ulg_class_get_property(const UlgClass* class_, const char* name);
