/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "jgui/core/context.h"

#include <stddef.h>

#include "jgui/core/error-code.h"
#include "jgui/core/id.h"

#include "jgui/private/misc/assert.h"

#include "common/cmocka.h"

/** jg_context_load should correctly load plugins. */
static void test_context_add_class(void** state) {
	jg_context* context = *state;
	const jg_class* test_class =
	    jg_context_get_class(context, JG_ID("test_namespace", "test_class"));
	assert_non_null(test_class);
}

static void error_handler(jg_error_code error_code, const char* message) {
	assert_string_equal(message, "Jean-GUI drinks a coffee.");
	assert(error_code == JG_ERROR_CLASS_UNDEFINED);
	function_called();
}

/** jg_set_error_handler should correctly set custom error handler. */
static void test_context_error_handler(void** state) {
	jg_context* context = *state;

	expect_function_call(error_handler);
	jg_error(
	    context, JG_ERROR_CLASS_UNDEFINED, "Jean-GUI drinks a %s.", "coffee");
}

static void test_plugin(jg_context* context) {
	jg_context_add_class(context,
	                     JG_ID("test_namespace", "test_class"),
	                     JG_ID_NULL,
	                     sizeof(char));
	jg_set_error_handler(context, error_handler);
}

static int setup(void** state) {
	*state = jg_context_load((jg_plugin[]) {test_plugin, NULL});
	return 0;
}

static int teardown(void** state) {
	jg_context_free(*state);
	return 0;
}

jg_begin_tests(NULL, NULL)
	cmocka_unit_test_setup_teardown(test_context_add_class, setup, teardown),
	    cmocka_unit_test_setup_teardown(
	        test_context_error_handler, setup, teardown),
jg_end_tests()
