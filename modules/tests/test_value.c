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

#include <jgui/module.h>
#include <jgui/object.h>
#include <jgui/value.h>

#include "fixtures/user_model.h"
#include "common.h"

typedef bool (*jg_check_function_t)(jg_value);

static void test_false_checks(jg_value value, jg_check_function_t expected_true_check) {
    if(expected_true_check != jg_is_bool) {
        assert_false(jg_is_bool(value));
    }
    if(expected_true_check != jg_is_int) {
        assert_false(jg_is_int(value));
    }
    if(expected_true_check != jg_is_double) {
        assert_false(jg_is_double(value));
    }
    if(expected_true_check != jg_is_string) {
        assert_false(jg_is_string(value));
    }
    if(expected_true_check != jg_is_raw) {
        assert_false(jg_is_raw(value));
    }
    if(expected_true_check != jg_is_object) {
        assert_false(jg_is_object(value));
    }
}

/** jg_bool, jg_is_bool and jg_to_bool should correctly handle boolean values. */
void test_jg_value_bool(void** state) {
    (void)state;
    jg_value false_ = jg_bool(false);
    jg_value true_ = jg_bool(true);

    test_false_checks(false_, jg_is_bool);
    test_false_checks(true_, jg_is_bool);

    assert_true(jg_is_bool(false_));
    assert_true(jg_is_bool(true_));

    assert_true(jg_to_bool(false_) == false);
    assert_true(jg_to_bool(true_) == true);
}

/** jg_int, jg_is_int and jg_to_int should correctly handle boolean values. */
void test_jg_value_int(void** state) {
    (void)state;

    const int test_int = 42; // If bugs happen with edge cases, put them in regression tests.
    jg_value value = jg_int(test_int);

    test_false_checks(value, jg_is_int);
    assert_true(jg_is_int(value));
    assert_int_equal(jg_to_int(value), test_int);
}

/** jg_double, jg_is_double and jg_to_double should correctly handle boolean values. */
void test_jg_value_double(void** state) {
    (void)state;
    const double test_double = .42; // If bugs happen with edge cases, put them in regression tests.
    jg_value value = jg_double(test_double);

    test_false_checks(value, jg_is_double);
    assert_true(jg_is_double(value));
    assert_true(jg_to_double(value) == test_double);
}

/** jg_string, jg_is_string and jg_to_string should correctly handle boolean values. */
void test_jg_value_string(void** state) {
    (void)state;
    // If bugs happen with edge cases, put them in regression tests.
    const char* test_string = "Jean-Mi Mo";
    jg_value value = jg_string(test_string);

    test_false_checks(value, jg_is_string);
    assert_true(jg_is_string(value));
    assert_string_equal(jg_to_string(value), test_string);
}

/** jg_raw, jg_is_raw and jg_to_raw should correctly handle boolean values. */
void test_jg_value_raw(void** state) {
    (void)state;
    void* test_raw = (void*)(0xDEADBEEF); // RAW BEEF XD XD XD
    jg_value value = jg_raw(test_raw);

    test_false_checks(value, jg_is_raw);
    assert_true(jg_is_raw(value));
    assert_true(jg_to_raw(value) == test_raw);
}

/** jg_object, jg_is_object and jg_to_object should correctly handle boolean values. */
void test_jg_value_object(void** state) {
    (void)state;
    jg_context* context = jg_context_load((jg_plugin[]) { user_model_plugin, NULL });
    const jg_class* admin_class = jg_context_get_class(context, user_model_ns, admin_class_id);
    void* test_object = jg_object_new(admin_class); 

    jg_value value = jg_object(test_object);

    test_false_checks(value, jg_is_object);
    assert_true(jg_is_object(value));
    assert_true(jg_to_object(value) == test_object);

    jg_object_free(test_object);
    jg_context_free(context);
}

void run_value_suite() {
    const struct CMUnitTest value_tests[] = {
        cmocka_unit_test(test_jg_value_bool),
        cmocka_unit_test(test_jg_value_int),
        cmocka_unit_test(test_jg_value_double),
        cmocka_unit_test(test_jg_value_string),
        cmocka_unit_test(test_jg_value_raw),
        cmocka_unit_test(test_jg_value_object)
    };

    cmocka_run_group_tests(value_tests, NULL, NULL);
}
