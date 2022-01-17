#include "test_object.h"

#define MU_MAIN
#include <minunit.h>

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    MU_RUN_SUITE(object_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}
