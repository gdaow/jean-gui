#include <check.h>
#include <assert.h>
#include <stdlib.h>

#include "uiligi/class.h"
#include "uiligi/object.h"

#include "test_object.h"

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
    UlgClass* class = ulg_class_create("TestClass", &test_object_constructor, &test_object_destructor);
    UlgObject* object = ulg_object_create(class);
    ulg_object_destroy(object);
}
END_TEST

Suite* object_suite(void)
{
    Suite* suite = suite_create("Objects");
    TCase* lifetime = tcase_create("Object Lifetime");

    tcase_add_test(lifetime, create_object);

    suite_add_tcase(suite, lifetime);
    return suite;
}
