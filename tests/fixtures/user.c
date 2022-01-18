#include <stdlib.h>

#include <uiligi/object.h>

#include "user.h"

static PermissionFlags _get_default_permissions() {
    return PERM_CAN_LOGIN;
}

static UlgValue _get_name(const UlgObject* object) {
    const User* user = (void *)object;
    return ulg_value_from_str(user->name);
}

static void _set_name(UlgObject* object, UlgValue value) {
    User* user = (void *)object;
    user->name = ulg_value_to_str(value);
}

static UlgValue _get_team(const UlgObject* object) {
    const User* user = (void *)object;
    // TODO : Implement this.
    return ulg_value_from_str(user->name);
}

static void _set_team(UlgObject* object, UlgValue value) {
    User* user = (void *)object;
    user->name = ulg_value_to_str(value);
}

const UlgClass* user_type(UlgClassFactory* factory) {
    UlgClass* class_ = ulg_class_declare(
        factory,
        "User",
        sizeof(User),
        ulg_object_type,
        sizeof(UserVT)
    );

    ulg_class_add_property(class_, "name", _get_name, _set_name);
    ulg_class_add_property(class_, "team", _get_team, _set_team);

    UserVT* vtable = ulg_class_edit_vtable(class_);
    vtable->get_default_permissions = _get_default_permissions;
    return class_;
}

PermissionFlags user_get_default_permissions(User* user) {
    return ((const UserVT*)ulg_object_vtable((UlgObject*)user))->get_default_permissions();
}
