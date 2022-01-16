#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <uiligi/class.h>

#include "utils/hashmap.h"
#include "class_impl.h"

uint64_t _property_hash(const void* item, uint64_t seed0, uint64_t seed1) {
    const UlgProperty* property = item;
    const char* name = property->name;
    return hashmap_sip(name, strlen(name), seed0, seed1);
}

int _property_compare(const void* a, const void* b, void* udata) {
    const UlgProperty *property_a = a;
    const UlgProperty *property_b = b;
    return strcmp(property_a->name, property_b->name);
}

UlgClass* ulg_class_create(
    const char* name,
    size_t size,
    UlgInitialize initialize,
    UlgCleanup cleanup,
    UlgClass* parent
) {
    UlgClass* result = malloc(sizeof(UlgClass));
    struct hashmap* properties = hashmap_new(
        sizeof(UlgProperty),
        0, 0, 0,
        &_property_hash,
        &_property_compare,
        NULL,
        NULL
    );

    size_t offset = 0;
    if(parent) {
        offset = parent->size;
    }

    UlgClass class_initializer = {
        .name = name,
        .size = size,
        .offset = offset,
        .initialize = initialize,
        .cleanup = cleanup,
        .parent = parent,
        .properties = properties
    };

    memcpy(result, &class_initializer, sizeof(UlgClass));

    return result;
}

void ulg_class_add_property(UlgClass* ulg_class, const char* name, UlgGetter getter, UlgSetter setter) {
    hashmap_set(
        ulg_class->properties, &(UlgProperty){
        .name = name,
        .getter = getter,
        .setter = setter
    });
}

UlgProperty* ulg_class_get_property(UlgClass* class_, const char* name, UlgClass** owner) {
    while(class_) {
        UlgProperty* result = hashmap_get(
            class_->properties,
            &(UlgProperty){ .name=name }
        );

        if(result) {
            *owner = class_;
            return result;
        }

        class_ = class_->parent;
    }
    
    assert(0); // TODO: Handle error.
}
