/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include <stdlib.h>
#include <string.h>

#include <jgui/core/context.h>
#include <jgui/core/error_code.h>

#include "jgui/private/misc/assert.h"

#include "common.h"

/** Setting a custom error handler when building a context should call it afterwards when
  * an error is raised.
  */
static void error_handler(jg_error_code error_code, const char* message) {
    assert_string_equal(message, "Jean-GUI drinks a coffee.");
    assert(error_code == JG_ERROR_CLASS_UNDEFINED);
    function_called();
}

static void error_handler_plugin(jg_context* context) {
    jg_set_error_handler(context, error_handler);
}

static void test_error_handler(void **state) {
    (void)state;
    jg_context* context = jg_context_load((jg_plugin[]) { error_handler_plugin, NULL });

    expect_function_call(error_handler);
    jg_error(context, JG_ERROR_CLASS_UNDEFINED, "Jean-GUI drinks a %s.", "coffee");

    jg_context_free(context);
}

/** Unit tests regarding the jg_context object */
void run_context_suite() {
    const struct CMUnitTest tests[] = {
         cmocka_unit_test(test_error_handler),
    };

    cmocka_run_group_tests(tests, NULL, NULL);
}
