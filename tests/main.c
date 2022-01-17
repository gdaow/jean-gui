#include <check.h>
#include "test_object.h"

int main(int argc, char** argv) {
    int number_failed;
    SRunner* runner = srunner_create(object_suite());
    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (number_failed == 0) ? 0 : -1;
}
