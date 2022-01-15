#include <stdlib.h>
#include "uiligi/object.h"

#include "class_impl.h"
#include "object_impl.h"


UlgObject* ulg_object_create(UlgClass* ulg_class) {
    UlgObject* result = malloc(sizeof(UlgObject));
    result->klass = ulg_class;
    result-> data = ulg_class->constructor();
    return result;
}

void ulg_object_destroy(UlgObject* object) {
    UlgClass* object_class = object->klass;
    object_class->destructor(object->data);
    free(object);
}
