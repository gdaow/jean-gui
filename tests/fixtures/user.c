#include <stdlib.h>
#include "user.h"
#include "uiligi/class.h"

struct User {
    const char* name;
} User;

void* user_constructor() {
    return malloc(sizeof(User));
}

void user_destructor(void* data) {
    free(data);
}

struct UlgClass* ulg_create_user_class() {
    UlgClass* user_class = ulg_class_create("User", &user_constructor, &user_destructor);
    return user_class;
}
