#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <uiligi/object.h>

#include "class_impl.h"
#include "utils/hashmap.h"

typedef struct _UlgObjectHeader {
    const UlgClass* class_;
    char data[];
} UlgObjectHeader;

void _object_initialize(const UlgClass* class_, char* data);
static UlgObjectHeader* _get_header(UlgObject* object) { return (UlgObjectHeader*)object - 1; }
static const UlgObjectHeader* _get_header_const(const UlgObject* object) { return (const UlgObjectHeader*)object - 1; }


UlgObject* ulg_object_new(const UlgClass* class_) {
    UlgObjectHeader* object = malloc(sizeof(UlgObjectHeader) + class_->data_size);
    object->class_ = class_;
    // Recursively initialize hierarchy, parent-first.
    // Return a pointer to the data, we offset the pointer to get the header each time.
    return (UlgObject*)(object + 1);
}

void ulg_object_free(UlgObject* object) {
    UlgObjectHeader* header = _get_header(object);
    free(header);
}

UlgValue ulg_object_get(const UlgObject* object, const char* property_name) {
    const UlgObjectHeader* header = _get_header_const(object);
    const UlgClass* class_ = header->class_;
    const UlgProperty* property = _ulg_class_get_property(class_, property_name);
    return property->getter(object);
}

void ulg_object_set(UlgObject* object, const char* property_name, UlgValue value) {
    UlgObjectHeader* header = _get_header(object);
    const UlgClass* class_ = header->class_;
    const UlgProperty* property = _ulg_class_get_property(class_, property_name);
    property->setter(object, value);
}
