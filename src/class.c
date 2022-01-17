#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <uiligi/class.h>

#include "class_impl.h"

void* ulg_class_edit_vtable(UlgClass* class_) {
    return class_->vtable;
}

void ulg_class_add_property(UlgClass* class_, const char* name, UlgGetter getter, UlgSetter setter) {
    hashmap_set(
        class_->properties,
        &(UlgProperty){
            .name = name,
            .getter = getter,
            .setter = setter
        }
    );
}

static uint64_t _property_hash(const void* item, uint64_t seed0, uint64_t seed1);
static int _property_compare(const void* a, const void* b, void* udata);
static bool _copy_parent_properties(const void* item, void* udata);

UlgClass* _ulg_class_new(
    const char* name,
    size_t data_size,
    const UlgClass* parent,
    size_t vtable_size
) {
    // vtable size can not be less than the parent one.
    assert(!parent || parent->vtable_size <= vtable_size);

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
        sizeof(UlgProperty),
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
}

void _ulg_class_free(UlgClass* class_) {
    hashmap_free(class_->properties);
    free(class_);
}

const UlgProperty* _ulg_class_get_property(const UlgClass* class_, const char* name) {
    UlgProperty* result = hashmap_get(
        class_->properties,
        &(UlgProperty){ .name = name }
    );
    assert(result); // TODO: Handle error.
    return result;
}

static uint64_t _property_hash(const void* item, uint64_t seed0, uint64_t seed1) {
    const UlgProperty* property = item;
    const char* name = property->name;
    return hashmap_sip(name, strlen(name), seed0, seed1);
}

static int _property_compare(const void* a, const void* b, void* udata) {
    (void)udata;
    const UlgProperty *property_a = a;
    const UlgProperty *property_b = b;
    return strcmp(property_a->name, property_b->name);
}

static bool _copy_parent_properties(const void* item, void* udata) {
    struct hashmap* child_properties = udata;
    UlgProperty copy;
    memcpy(&copy, item, sizeof(UlgProperty));
    hashmap_set(child_properties, &copy);
    return true;
}
