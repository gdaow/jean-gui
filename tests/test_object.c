#include <check.h>
#include <assert.h>
#include <stdlib.h>

#include "uiligi/class.h"
#include "uiligi/object.h"

#include "test_object.h"
#include "fixtures/user.h"

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

START_TEST (set_property) {
    UlgClass* user_class = ulg_create_user_class();
    UlgObject* user = ulg_object_create(user_class);

    const char* user_name = "Henri Krasucki";
    ulg_object_set(user, "name", ulg_value_from_str(user_name));

    UlgValue property_value = ulg_object_get(user, "name");
    const char* get_name = ulg_value_to_str(property_value);
    int is_same = strcasecmp(get_name, user_name) == 0;
    assert(is_same);

    ulg_object_destroy(user);
    free(user_class);
}
END_TEST

Suite* object_suite(void)
{
    Suite* suite = suite_create("Objects");
    TCase* lifetime = tcase_create("Object Lifetime");

    tcase_add_test(lifetime, create_object);
    tcase_add_test(lifetime, set_property);

    suite_add_tcase(suite, lifetime);
    return suite;
}
