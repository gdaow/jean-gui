#include <stdlib.h>

#include <uiligi/class_registry.h>
#include <uiligi/class.h>

#include "user.h"

static void set_name(UlgObject* object, UlgValue value) {
    User* user = (void *)object;
    user->name = ulg_value_to_str(value);
}

static UlgValue get_name(const UlgObject* object) {
    const User* user = (void *)object;
    return ulg_value_from_str(user->name);
}

const UlgClass* user(UlgClassFactory* factory) {
    UlgClass* class_ = ulg_class_declare(
        factory,
        "User",
        sizeof(User),
        NULL,
        0
    );

    ulg_class_add_property(class_, "name", get_name, set_name);

    return class_;
}
