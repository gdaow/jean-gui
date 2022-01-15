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
    UlgConstructor constructor;
    UlgDestructor destructor;
    struct hashmap* properties;
};

UlgProperty* ulg_class_get_property(UlgClass* class, const char* name);
