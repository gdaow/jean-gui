#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <uiligi/object.h>

#include "utils/hashmap.h"

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

struct UlgObject {
    UlgClass* class_;
    char data[];
};

void _object_initialize(UlgClass* class_, char* data);
uint64_t _property_hash(const void* item, uint64_t seed0, uint64_t seed1);
int _property_compare(const void* a, const void* b, void* udata);


UlgClass* ulg_class_create(
    const char* name,
    size_t size,
    UlgInitialize initialize,
    UlgCleanup cleanup,
    UlgClass* parent
) {
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

    return memcpy(
        malloc(sizeof(UlgClass)),
        &(UlgClass) {
            .name = name,
            .size = size,
            .offset = offset,
            .initialize = initialize,
            .cleanup = cleanup,
            .parent = parent,
            .properties = properties
        },
        sizeof(UlgClass)
    );
}

void ulg_class_destroy(UlgClass* class_) {
    hashmap_free(class_->properties);
    free(class_);
}

void ulg_class_add_property(UlgClass* class_, const char* name, UlgGetter getter, UlgSetter setter) {
    hashmap_set(
        class_->properties, &(UlgProperty){
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

UlgObject* ulg_object_create(UlgClass* class_) {
    UlgObject* result = malloc(sizeof(UlgObject) + class_->size);
    result->class_ = class_;
    _object_initialize(class_, result->data);
    return result;
}

void ulg_object_destroy(UlgObject* object) {
    UlgClass* class_ = object->class_;
    char* data = object->data;

    while(class_) {
        class_->cleanup(data + class_->offset);
        class_ = class_->parent;
    }
    free(object);
}

void ulg_object_set(UlgObject* object, const char* property_name, UlgValue value) {
    UlgClass* object_class = object->class_;
    UlgClass* owner;
    UlgProperty* property = ulg_class_get_property(object_class, property_name, &owner);
    property->setter(object, object->data + owner->offset, value);
}

UlgValue ulg_object_get(UlgObject* object, const char* property_name) {
    UlgClass* object_class = object->class_;
    UlgClass* owner;
    UlgProperty* property = ulg_class_get_property(object_class, property_name, &owner);
    return property->getter(object, object->data + owner->offset);
}

void _object_initialize(UlgClass* class_, char* data) {
    UlgClass* parent = class_->parent;
    if(parent) {
        _object_initialize(parent, data);
    }

    class_->initialize(data + class_->offset);
}

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
