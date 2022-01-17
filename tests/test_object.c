#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <minunit.h>

#include <uiligi/object.h>

#include "test_object.h"
#include "fixtures/user.h"
#include "fixtures/admin.h"

/**
 * UlgClass should be able to instanciate object dynamically.
 */

MU_TEST(create_object) {
    UlgContext* context = ulg_context_new();
    const UlgClass* object_ = ulg_class_get(context, ulg_object);
    assert(object_);
    ulg_class_register(context, user);
    ulg_class_register(context, admin);
    const UlgClass* user_class = ulg_class_get(context, user);
    UlgObject* object = ulg_object_new(user_class);
    ulg_object_free(object);
    ulg_context_free(context);
}

void test_set_user_name(const UlgClass* class_) {
    UlgObject* obj = ulg_object_new(class_);
    const char* obj_name = "Henri Krasucki";
    ulg_object_set(obj, "name", ulg_value_from_str(obj_name));

    UlgValue property_value = ulg_object_get(obj, "name");
    const char* get_name = ulg_value_to_str(property_value);
    int is_same = strcasecmp(get_name, obj_name) == 0;
    mu_check(is_same);
    ulg_object_free(obj);
}

MU_TEST(set_property) {
    UlgContext* context = ulg_context_new();
    ulg_class_register(context, user);
    ulg_class_register(context, admin);
    const UlgClass* user_class = ulg_class_get(context, user);
    test_set_user_name(user_class);
    ulg_context_free(context);
}

MU_TEST(virtual_methods) {
    UlgContext* context = ulg_context_new();
    ulg_class_register(context, user);
    ulg_class_register(context, admin);
    const UlgClass* user_class = ulg_class_get(context, user);
    const UlgClass* admin_class = ulg_class_get(context, admin);
    User* user_ = (User*)ulg_object_new(user_class);
    User* admin_ = (User*)ulg_object_new(admin_class);

    user_set_default_name(user_);
    user_set_default_name(admin_);

    mu_check(strcasecmp(user_->name, "default user name") == 0);
    mu_check(strcasecmp(admin_->name, "default admin name") == 0);

    ulg_object_free((UlgObject*)admin_);
    ulg_object_free((UlgObject*)user_);
    ulg_context_free(context);
}

MU_TEST(set_parent_property) {
    UlgContext* context = ulg_context_new();
    ulg_class_register(context, user);
    ulg_class_register(context, admin);
    const UlgClass* admin_class = ulg_class_get(context, admin);
    test_set_user_name(admin_class);
    ulg_context_free(context);
}

MU_TEST_SUITE(object_suite) {
    MU_RUN_TEST(create_object);
    MU_RUN_TEST(set_property);
    MU_RUN_TEST(set_parent_property);
    MU_RUN_TEST(virtual_methods);
}
