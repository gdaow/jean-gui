/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#define MU_MAIN
#include <minunit.h>

void value_suite();
void object_suite();
void template_suite();

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    MU_RUN_SUITE(value_suite);
    MU_RUN_SUITE(object_suite);
    MU_RUN_SUITE(template_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}
