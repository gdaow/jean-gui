#include <check.h>
#include <assert.h>

#include "test_object.h"

START_TEST (test_define_property)
{
}
END_TEST

Suite* object_suite(void)
{
    Suite* suite = suite_create("Objects");
    TCase* creation_case = tcase_create("Object Creation");
    tcase_add_test(creation_case, test_define_property);
    suite_add_tcase(suite, creation_case);
    return suite;
}
