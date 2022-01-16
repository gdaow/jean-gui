#include <stdlib.h>

#include <uiligi/object.h>

#include "admin.h"
#include "user.h"

typedef struct {
    const char* role;
} Admin;

void admin_initialize(void* data) {

}

void admin_cleanup(void* data) {

}

UlgValue admin_get_role(UlgObject* object, void* data) {
    Admin* admin = data;
    return ulg_value_from_str(admin->role);
}

void admin_set_role(UlgObject* object, void* data, UlgValue value) {
    Admin* admin = data;
    admin->role = ulg_value_to_str(value);
}

struct UlgClass* ulg_create_admin_class() {
    UlgClass* admin_class = ulg_class_create(
        "Admin",
        sizeof(Admin),
        &admin_initialize,
        &admin_cleanup,
        ulg_create_user_class()
    );
    ulg_class_add_property(admin_class, "role", &admin_get_role, &admin_set_role);
    return admin_class;
}
