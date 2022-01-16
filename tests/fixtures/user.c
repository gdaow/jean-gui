#include <stdlib.h>

#include <uiligi/object.h>

#include "user.h"

typedef struct User {
    const char* name;
} User;

void user_initialize(void* data) { }

void user_cleanup(void* data) {}

void user_set_name(UlgObject* object, void* data, UlgValue value) {
    User* user = data;
    user->name = ulg_value_to_str(value);
}

UlgValue user_get_name(const UlgObject* object, const void* data) {
    const User* user = data;
    return ulg_value_from_str(user->name);
}

struct UlgClass* ulg_create_user_class() {
    UlgClass* user_class = ulg_class_create(
        "User",
        sizeof(User),
        &user_initialize,
        &user_cleanup,
        NULL
    );
    ulg_class_add_property(user_class, "name", &user_get_name, &user_set_name);
    return user_class;
}
