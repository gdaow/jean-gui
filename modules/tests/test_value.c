/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#include <minunit.h>

#include <jgui/module.h>
#include <jgui/object.h>
#include <jgui/value.h>

#include "fixtures/user_model.h"

typedef bool (*jg_check_function_t)(jg_value);

static void _test_false_checks(jg_value value, jg_check_function_t expected_true_check) {
    if(expected_true_check != jg_is_bool)
        mu_check(!jg_is_bool(value));
    if(expected_true_check != jg_is_int)
        mu_check(!jg_is_int(value));
    if(expected_true_check != jg_is_double)
        mu_check(!jg_is_double(value));
    if(expected_true_check != jg_is_string)
        mu_check(!jg_is_string(value));
    if(expected_true_check != jg_is_raw)
        mu_check(!jg_is_raw(value));
    if(expected_true_check != jg_is_object)
        mu_check(!jg_is_object(value));
}

/** jg_bool, jg_is_bool and jg_to_bool should correctly handle boolean values. */
MU_TEST(test_jg_value_bool) {
    jg_value false_ = jg_bool(false);
    jg_value true_ = jg_bool(true);

    _test_false_checks(false_, jg_is_bool);
    _test_false_checks(true_, jg_is_bool);

    mu_check(jg_is_bool(false_));
    mu_check(jg_is_bool(true_));

    mu_check(jg_to_bool(false_) == false);
    mu_check(jg_to_bool(true_) == true);
}

/** jg_int, jg_is_int and jg_to_int should correctly handle boolean values. */
MU_TEST(test_jg_value_int) {
    const int test_int = 42; // If bugs happen with edge cases, put them in regression tests.
    jg_value value = jg_int(test_int);

    _test_false_checks(value, jg_is_int);
    mu_check(jg_is_int(value));
    mu_assert_int_eq(jg_to_int(value), test_int);
}

/** jg_double, jg_is_double and jg_to_double should correctly handle boolean values. */
MU_TEST(test_jg_value_double) {
    const double test_double = .42; // If bugs happen with edge cases, put them in regression tests.
    jg_value value = jg_double(test_double);

    _test_false_checks(value, jg_is_double);
    mu_check(jg_is_double(value));
    mu_assert_double_eq(jg_to_double(value), test_double);
}

/** jg_string, jg_is_string and jg_to_string should correctly handle boolean values. */
MU_TEST(test_jg_value_string) {
    // If bugs happen with edge cases, put them in regression tests.
    const char* test_string = "Jean-Mi Mo";
    jg_value value = jg_string(test_string);

    _test_false_checks(value, jg_is_string);
    mu_check(jg_is_string(value));
    mu_assert_string_eq(jg_to_string(value), test_string);
}

/** jg_raw, jg_is_raw and jg_to_raw should correctly handle boolean values. */
MU_TEST(test_jg_value_raw) {
    void* test_raw = (void*)(0xDEADBEEF); // RAW BEEF XD XD XD
    jg_value value = jg_raw(test_raw);

    _test_false_checks(value, jg_is_raw);
    mu_check(jg_is_raw(value));
    mu_check(jg_to_raw(value) == test_raw);
}

/** jg_object, jg_is_object and jg_to_object should correctly handle boolean values. */
MU_TEST(test_jg_value_object) {
    jg_context* context = jg_context_load((jg_plugin[]) { user_model_plugin, NULL });
    const jg_class* admin_class = jg_context_get_class(context, user_model_ns, admin_class_id);
    void* test_object = jg_object_new(admin_class); 

    jg_value value = jg_object(test_object);

    _test_false_checks(value, jg_is_object);
    mu_check(jg_is_object(value));
    mu_check(jg_to_object(value) == test_object);

    jg_object_free(test_object);
    jg_context_free(context);
}

MU_TEST_SUITE(value_suite) {
    MU_RUN_TEST(test_jg_value_bool);
    MU_RUN_TEST(test_jg_value_int);
    MU_RUN_TEST(test_jg_value_double);
    MU_RUN_TEST(test_jg_value_string);
    MU_RUN_TEST(test_jg_value_raw);
    MU_RUN_TEST(test_jg_value_object);
}
