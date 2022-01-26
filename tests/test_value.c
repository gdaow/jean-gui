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

#include <mezo/value.h>
#include <mezo/object.h>

#include "fixtures/user_model.h"

typedef bool (*mz_check_function_t)(mz_value);

static void _test_false_checks(mz_value value, mz_check_function_t expected_true_check) {
    if(expected_true_check != mz_is_bool)
        mu_check(!mz_is_bool(value));
    if(expected_true_check != mz_is_int)
        mu_check(!mz_is_int(value));
    if(expected_true_check != mz_is_double)
        mu_check(!mz_is_double(value));
    if(expected_true_check != mz_is_string)
        mu_check(!mz_is_string(value));
    if(expected_true_check != mz_is_raw)
        mu_check(!mz_is_raw(value));
    if(expected_true_check != mz_is_object)
        mu_check(!mz_is_object(value));
}

/** mz_bool, mz_is_bool and mz_to_bool should correctly handle boolean values. */
MU_TEST(test_mz_value_bool) {
    mz_value false_ = mz_bool(false);
    mz_value true_ = mz_bool(true);

    _test_false_checks(false_, mz_is_bool);
    _test_false_checks(true_, mz_is_bool);

    mu_check(mz_is_bool(false_));
    mu_check(mz_is_bool(true_));

    mu_check(mz_to_bool(false_) == false);
    mu_check(mz_to_bool(true_) == true);
}

/** mz_int, mz_is_int and mz_to_int should correctly handle boolean values. */
MU_TEST(test_mz_value_int) {
    const int test_int = 42; // If bugs happen with edge cases, put them in regression tests.
    mz_value value = mz_int(test_int);

    _test_false_checks(value, mz_is_int);
    mu_check(mz_is_int(value));
    mu_assert_int_eq(mz_to_int(value), test_int);
}

/** mz_double, mz_is_double and mz_to_double should correctly handle boolean values. */
MU_TEST(test_mz_value_double) {
    const double test_double = .42; // If bugs happen with edge cases, put them in regression tests.
    mz_value value = mz_double(test_double);

    _test_false_checks(value, mz_is_double);
    mu_check(mz_is_double(value));
    mu_assert_double_eq(mz_to_double(value), test_double);
}

/** mz_string, mz_is_string and mz_to_string should correctly handle boolean values. */
MU_TEST(test_mz_value_string) {
    // If bugs happen with edge cases, put them in regression tests.
    const char* test_string = "Jean-Mi Mo";
    mz_value value = mz_string(test_string);

    _test_false_checks(value, mz_is_string);
    mu_check(mz_is_string(value));
    mu_assert_string_eq(mz_to_string(value), test_string);
}

/** mz_raw, mz_is_raw and mz_to_raw should correctly handle boolean values. */
MU_TEST(test_mz_value_raw) {
    void* test_raw = (void*)(0xDEADBEEF); // RAW BEEF XD XD XD
    mz_value value = mz_raw(test_raw);

    _test_false_checks(value, mz_is_raw);
    mu_check(mz_is_raw(value));
    mu_check(mz_to_raw(value) == test_raw);
}

/** mz_object, mz_is_object and mz_to_object should correctly handle boolean values. */
MU_TEST(test_mz_value_object) {
    mz_module* module = user_model_module_new();
    const mz_class* admin_class = mz_class_get(module, ADMIN);
    void* test_object = mz_object_new(admin_class); 

    mz_value value = mz_object(test_object);

    _test_false_checks(value, mz_is_object);
    mu_check(mz_is_object(value));
    mu_check(mz_to_object(value) == test_object);

    mz_object_free(test_object);
    mz_module_free(module);
}

MU_TEST_SUITE(value_suite) {
    MU_RUN_TEST(test_mz_value_bool);
    MU_RUN_TEST(test_mz_value_int);
    MU_RUN_TEST(test_mz_value_double);
    MU_RUN_TEST(test_mz_value_string);
    MU_RUN_TEST(test_mz_value_raw);
    MU_RUN_TEST(test_mz_value_object);
}
