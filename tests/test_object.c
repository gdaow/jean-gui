#include <check.h>
#include <assert.h>
#include <stdlib.h>

#include <uiligi/class_registry.h>
#include <uiligi/object.h>

#include "test_object.h"
#include "fixtures/user.h"
#include "fixtures/admin.h"

/**
 * UlgClass should be able to instanciate object dynamically.
 */

START_TEST (create_object) {
    UlgClassRegistry* registry = ulg_class_registry_new();
    const UlgClass* user_class = ulg_class_get(registry, user);
    UlgObject* object = ulg_object_new(user_class);
    ulg_object_free(object);
    ulg_class_registry_free(registry);
}

void test_set_user_name(const UlgClass* class_) {
    UlgObject* obj = ulg_object_new(class_);
    const char* obj_name = "Henri Krasucki";
    ulg_object_set(obj, "name", ulg_value_from_str(obj_name));

    UlgValue property_value = ulg_object_get(obj, "name");
    const char* get_name = ulg_value_to_str(property_value);
    int is_same = strcasecmp(get_name, obj_name) == 0;
    assert(is_same);
    ulg_object_free(obj);
}

START_TEST (set_property) {
    UlgClassRegistry* registry = ulg_class_registry_new();
    const UlgClass* user_class = ulg_class_get(registry, user);
    test_set_user_name(user_class);
    ulg_class_registry_free(registry);
}
END_TEST

START_TEST (set_parent_property) {
    UlgClassRegistry* registry = ulg_class_registry_new();
    const UlgClass* admin_class = ulg_class_get(registry, admin);
    test_set_user_name(admin_class);
    ulg_class_registry_free(registry);
}
END_TEST

Suite* object_suite(void)
{
    Suite* suite = suite_create("Objects");
    TCase* lifetime = tcase_create("Object Lifetime");

    tcase_add_test(lifetime, create_object);
    tcase_add_test(lifetime, set_property);
    tcase_add_test(lifetime, set_parent_property);

    suite_add_tcase(suite, lifetime);
    return suite;
}
