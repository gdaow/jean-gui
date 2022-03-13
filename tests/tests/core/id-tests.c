/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <stddef.h>

#include "jgui/private/misc/assert.h"

#include "common/cmocka.h"
#include <jgui/core/id.h>

/** jg_id_is_none should return true only for JG_ID_NONE. */
static void test_jg_id_none(void** state) {
	(void)state;
	assert(jg_id_is_null(JG_ID_NULL));
	assert(!jg_id_is_null(JG_ID("namespace", "name")));
}

/** Id creation should fail for invalid namespace / name pairs. */
static void test_jg_id_create(void** state) {
	(void)state;
	expect_assert_failure(JG_ID(NULL, NULL));

	expect_assert_failure(JG_ID("namespace", NULL));
	expect_assert_failure(JG_ID(NULL, "name"));

	expect_assert_failure(JG_ID("", ""));
	expect_assert_failure(JG_ID("", "name"));
	expect_assert_failure(JG_ID("namespace", ""));

	JG_ID("namespace", "name");
}

jg_begin_tests(NULL, NULL)
	cmocka_unit_test(test_jg_id_none), cmocka_unit_test(test_jg_id_create),
jg_end_tests()
