#define MU_MAIN
#include <minunit.h>

void object_suite();
void template_suite();

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    MU_RUN_SUITE(object_suite);
    MU_RUN_SUITE(template_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}
