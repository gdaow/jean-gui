#include <stdlib.h>

#include <uiligi/class_registry.h>
#include <uiligi/class.h>

#include "user.h"

static void _set_default_name(User* user) {
    user->name = "default user name";
}

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
        ulg_object,
        sizeof(UserVT)
    );

    ulg_class_add_property(class_, "name", get_name, set_name);
    UserVT* vtable = ulg_class_edit_vtable(class_);
    vtable->set_default_name = _set_default_name;
    return class_;
}

void user_set_default_name(User* user) {
    const UserVT* vt = ulg_object_vtable((UlgObject*)user);
    vt->set_default_name(user);
}