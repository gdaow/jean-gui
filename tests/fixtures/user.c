#include <stdlib.h>

#include "uiligi/class.h"
#include "uiligi/object.h"

#include "user.h"

typedef struct User {
    const char* name;
} User;

void* user_constructor() {
    return malloc(sizeof(User));
}

void user_destructor(void* data) {
    free(data);
}

void user_set_name(void* object, UlgValue value) {
    User* user = object;
    user->name = ulg_value_to_str(value);
}

UlgValue user_get_name(void* object) {
    User* user = object;
    return ulg_value_from_str(user->name);
}

struct UlgClass* ulg_create_user_class() {
    UlgClass* user_class = ulg_class_create("User", &user_constructor, &user_destructor);
    ulg_class_add_property(user_class, "name", &user_get_name, &user_set_name);
    return user_class;
}
