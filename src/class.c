#include <stdlib.h>

#include "uiligi/class.h"
#include "class_impl.h"

UlgClass* ulg_class_create(const char* name, UlgConstructor constructor, UlgDestructor destructor) {
    UlgClass* result = malloc(sizeof(UlgClass));
    result->name = name;
    result->constructor = constructor;
    result->destructor = destructor;
    return result;
}
