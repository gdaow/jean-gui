#pragma once

#include <uiligi/class.h>

struct hashmap;

typedef struct {
    const char* name;
    UlgGetter getter;
    UlgSetter setter;
} UlgProperty;


struct UlgClass {
    const char* name;
    size_t size;
    size_t offset;
    UlgInitialize initialize;
    UlgCleanup cleanup;
    UlgClass* parent;
    struct hashmap* properties;
};

UlgProperty* ulg_class_get_property(UlgClass* class, const char* name, UlgClass** owner);
