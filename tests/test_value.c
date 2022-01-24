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

#include <uiligi/value.h>
#include <uiligi/object.h>

#include "fixtures/user_model.h"

typedef bool (*UlgCheckFunction)(UlgValue);

static void _test_false_checks(UlgValue value, UlgCheckFunction expected_true_check) {
    if(expected_true_check != ulg_is_bool)
        mu_check(!ulg_is_bool(value));
    if(expected_true_check != ulg_is_int)
        mu_check(!ulg_is_int(value));
    if(expected_true_check != ulg_is_double)
        mu_check(!ulg_is_double(value));
    if(expected_true_check != ulg_is_string)
        mu_check(!ulg_is_string(value));
    if(expected_true_check != ulg_is_raw)
        mu_check(!ulg_is_raw(value));
    if(expected_true_check != ulg_is_object)
        mu_check(!ulg_is_object(value));
}

/** ulg_bool, ulg_is_bool and ulg_to_bool should correctly handle boolean values. */
MU_TEST(test_ulg_value_bool) {
    UlgValue false_ = ulg_bool(false);
    UlgValue true_ = ulg_bool(true);

    _test_false_checks(false_, ulg_is_bool);
    _test_false_checks(true_, ulg_is_bool);

    mu_check(ulg_is_bool(false_));
    mu_check(ulg_is_bool(true_));

    mu_check(ulg_to_bool(false_) == false);
    mu_check(ulg_to_bool(true_) == true);
}

/** ulg_int, ulg_is_int and ulg_to_int should correctly handle boolean values. */
MU_TEST(test_ulg_value_int) {
    const int test_int = 42; // If bugs happen with edge cases, put them in regression tests.
    UlgValue value = ulg_int(test_int);

    _test_false_checks(value, ulg_is_int);
    mu_check(ulg_is_int(value));
    mu_assert_int_eq(ulg_to_int(value), test_int);
}

/** ulg_double, ulg_is_double and ulg_to_double should correctly handle boolean values. */
MU_TEST(test_ulg_value_double) {
    const double test_double = .42; // If bugs happen with edge cases, put them in regression tests.
    UlgValue value = ulg_double(test_double);

    _test_false_checks(value, ulg_is_double);
    mu_check(ulg_is_double(value));
    mu_assert_double_eq(ulg_to_double(value), test_double);
}

/** ulg_string, ulg_is_string and ulg_to_string should correctly handle boolean values. */
MU_TEST(test_ulg_value_string) {
    // If bugs happen with edge cases, put them in regression tests.
    const char* test_string = "Jean-Mi Mo";
    UlgValue value = ulg_string(test_string);

    _test_false_checks(value, ulg_is_string);
    mu_check(ulg_is_string(value));
    mu_assert_string_eq(ulg_to_string(value), test_string);
}

/** ulg_raw, ulg_is_raw and ulg_to_raw should correctly handle boolean values. */
MU_TEST(test_ulg_value_raw) {
    void* test_raw = (void*)(0xDEADBEEF); // RAW BEEF XD XD XD
    UlgValue value = ulg_raw(test_raw);

    _test_false_checks(value, ulg_is_raw);
    mu_check(ulg_is_raw(value));
    mu_check(ulg_to_raw(value) == test_raw);
}

/** ulg_object, ulg_is_object and ulg_to_object should correctly handle boolean values. */
MU_TEST(test_ulg_value_object) {
    UlgModule* module = user_model_module_new();
    const UlgClass* admin_class = ulg_class_get(module, ADMIN);
    void* test_object = ulg_object_new(admin_class); 

    UlgValue value = ulg_object(test_object);

    _test_false_checks(value, ulg_is_object);
    mu_check(ulg_is_object(value));
    mu_check(ulg_to_object(value) == test_object);

    ulg_object_free(test_object);
    ulg_module_free(module);
}

MU_TEST_SUITE(value_suite) {
    MU_RUN_TEST(test_ulg_value_bool);
    MU_RUN_TEST(test_ulg_value_int);
    MU_RUN_TEST(test_ulg_value_double);
    MU_RUN_TEST(test_ulg_value_string);
    MU_RUN_TEST(test_ulg_value_raw);
    MU_RUN_TEST(test_ulg_value_object);
}
