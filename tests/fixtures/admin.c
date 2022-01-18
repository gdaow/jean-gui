#include <stdlib.h>

#include <uiligi/object.h>

#include "admin.h"
#include "user.h"

static PermissionFlags _get_default_permissions() {
    return PERM_ALL;
}

static UlgValue get_role(const UlgObject* object) {
    const Admin* admin = (void *)object;
    return ulg_value_from_str(admin->role);
}

static void set_role(UlgObject* object, UlgValue value) {
    Admin* admin = (void *)object;
    admin->role = ulg_value_to_str(value);
}

const UlgClass* admin_type(UlgClassFactory* factory) {
    UlgClass* class_ = ulg_class_declare(
        factory,
        "Admin",
        sizeof(Admin),
        user_type,
        0
    );

    ulg_class_add_property(class_, "role", get_role, set_role);
    UserVT* vtable = ulg_class_edit_vtable(class_);
    vtable->get_default_permissions = _get_default_permissions;

    return class_;
}
