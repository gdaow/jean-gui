#include <stdlib.h>
#include "uiligi/object.h"

#include "class_impl.h"
#include "object_impl.h"

void _ulg_object_initialize(UlgClass* class_, char* data);

UlgObject* ulg_object_create(UlgClass* class_) {
    UlgObject* result = malloc(sizeof(UlgObject) + class_->size);
    result->class_ = class_;
    _ulg_object_initialize(class_, result->data);
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

void _ulg_object_initialize(UlgClass* class_, char* data) {
    UlgClass* parent = class_->parent;
    if(parent) {
        _ulg_object_initialize(parent, data);
    }

    class_->initialize(data + class_->offset);
}
