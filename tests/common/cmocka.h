/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#ifndef JGUI_TESTS_COMMON_CMOCKA_H
#define JGUI_TESTS_COMMON_CMOCKA_H

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include "jgui/private/misc/assert.h"

#define jg_suite(setup, teardown, ...)      \
	int main(void) {                         \
		const struct CMUnitTest tests[] = { __VA_ARGS__}; \
		return _cmocka_run_group_tests("Tests", tests, sizeof(tests) / sizeof((tests)[0]), setup, teardown); \
	}

#define jg_test_st(test, setup, teardown) cmocka_unit_test_setup_teardown(test, setup, teardown)

#define jg_test(test) cmocka_unit_test(test)

#endif
