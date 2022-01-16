#include <check.h>
#include <assert.h>
#include <stdlib.h>

#include "uiligi/class.h"
#include "uiligi/object.h"

#include "test_object.h"
#include "fixtures/user.h"
#include "fixtures/admin.h"

/**
 * UlgClass should be able to instanciate object dynamically.
 */
typedef struct TestObject {
    int data;
} TestObject;

void* test_object_constructor() {
    TestObject* result = malloc(sizeof(TestObject));
    result->data = 0xB00B00;
    return result;
}

void test_object_destructor(void* object) {
    free(object);
}

START_TEST (create_object) {
    UlgClass* user_class = ulg_create_user_class();
    UlgObject* object = ulg_object_create(user_class);
    ulg_object_destroy(object);
    free(user_class);
}

void test_set_user_name(UlgClass* class_) {
    UlgObject* obj = ulg_object_create(class_);
    const char* obj_name = "Henri Krasucki";
    ulg_object_set(obj, "name", ulg_value_from_str(obj_name));

    UlgValue property_value = ulg_object_get(obj, "name");
    const char* get_name = ulg_value_to_str(property_value);
    int is_same = strcasecmp(get_name, obj_name) == 0;
    assert(is_same);
    ulg_object_destroy(obj);
}

START_TEST (set_property) {
    UlgClass* user_class = ulg_create_user_class();
    test_set_user_name(user_class);
    free(user_class);
}
END_TEST

START_TEST (set_parent_property) {
    UlgClass* admin_class = ulg_create_admin_class();
    test_set_user_name(admin_class);
    free(admin_class);
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
