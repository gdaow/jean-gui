/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <stdbool.h>
#include <stddef.h>

#include <jgui/object-model/arguments.h>
#include <jgui/object-model/value.h>

#include "jgui/private/misc/assert.h"

#include "common/cmocka.h"

static const double test_double = 42.24;

/** jg_object, jg_is_object and jg_to_object should correctly handle boolean values. */
static void test_arguments_pop(void** state) {
    (void)state;
    jg_arguments args = jg_arguments_new(
        jg_bool(true),
        jg_bool(false),
        jg_int(42),
        jg_double(test_double),
        jg_string("Henry Krascuky"),
        jg_raw((void*)0xDEADBEEF)
     );
    assert(jg_pop_bool(&args) == true);
    assert(jg_pop_bool(&args) == false);
    assert(jg_pop_int(&args) == 42);
    assert_double_equal(jg_pop_double(&args), test_double, .0);
    assert_string_equal(jg_pop_string(&args), "Henry Krascuky");
    assert(jg_pop_raw(&args) == (void*)0xDEADBEEF);
}

jg_begin_tests(NULL, NULL)
    cmocka_unit_test(test_arguments_pop),
jg_end_tests()

