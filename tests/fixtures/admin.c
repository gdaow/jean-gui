#include <stdlib.h>

#include <uiligi/object.h>

#include "admin.h"
#include "user.h"

static void set_default_name(User* user) {
    user->name = "default admin name";
}

static UlgValue get_role(const UlgObject* object) {
    const Admin* admin = (void *)object;
    return ulg_value_from_str(admin->role);
}

static void set_role(UlgObject* object, UlgValue value) {
    Admin* admin = (void *)object;
    admin->role = ulg_value_to_str(value);
}

const UlgClass* admin(UlgClassFactory* factory) {
    UlgClass* class_ = ulg_class_declare(
        factory,
        "Admin",
        sizeof(Admin),
        user,
        0
    );

    ulg_class_add_property(class_, "role", get_role, set_role);
    UserVT* vtable = ulg_class_edit_vtable(class_);
    vtable->set_default_name = set_default_name;

    return class_;
}
