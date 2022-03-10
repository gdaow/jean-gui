/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include <jgui/object-model/class.h>
#include "jgui/private/object-model/class.h"

#include <jgui/object-model/arguments.h>
#include <jgui/object-model/value.h>

#include "jgui/private/containers/vector.h"
#include "jgui/private/object-model/member.h"

#include "tests/common/cmocka.h"

static jg_value parent_getter(const void* object) {
    (void)object;
    return jg_string("parent");
}

static jg_value child_getter(const void* object) {
    (void)object;
    return jg_string("child");
}

static void get_property(jg_class* class_, const char* property_name, const char* expected_value) {
    const jg_member* property = jg_class_get_member(class_, property_name);
    assert_non_null(property);

    jg_value value = jg_property_get(property, NULL);
    assert_string_equal(jg_to_string(value), expected_value);
}


/** Properties should be correctly registered in class, and retrieved, including overriden ones. */
static void test_add_property(void** state) {
    (void)state;
    jg_class child_class;
    jg_class parent_class;

    jg_class_init(&parent_class, NULL, sizeof(char));
    jg_class_add_property(&parent_class, "parent_property", &parent_getter, NULL);
    jg_class_add_property(&parent_class, "overriden_property", &parent_getter, NULL);

    jg_class_init(&child_class, &parent_class, sizeof(char));
    jg_class_add_property(&child_class, "child_property", &child_getter, NULL);
    jg_class_add_property(&child_class, "overriden_property", &child_getter, NULL);

    get_property(&child_class, "parent_property", "parent");
    get_property(&child_class, "child_property", "child");
    get_property(&parent_class, "overriden_property", "parent");
    get_property(&child_class, "overriden_property", "child");

    jg_class_cleanup(&parent_class);
    jg_class_cleanup(&child_class);
}

static jg_value parent_method(jg_arguments* args) {
    (void)args;
    return jg_string("parent");
}

static jg_value child_method(jg_arguments* args) {
    (void)args;
    return jg_string("child");
}

static void call_method(jg_class* class_, const char* method_name, const char* expected_return) {
    const jg_member* method = jg_class_get_member(class_, method_name);
    assert_non_null(method);

    jg_value return_value = jg_method_call(method, &(jg_arguments) {0} );
    assert_string_equal(jg_to_string(return_value), expected_return);
}

/** Methods should be correctly registered in class, and retrieved, including overriden ones. */
static void test_add_method(void** state) {
    (void)state;
    jg_class child_class;
    jg_class parent_class;

    jg_class_init(&parent_class, NULL, sizeof(char));
    jg_class_add_method(&parent_class, "parent_method", &parent_method);
    jg_class_add_method(&parent_class, "overriden_method", &parent_method);

    jg_class_init(&child_class, &parent_class, sizeof(char));
    jg_class_add_method(&child_class, "child_method", &child_method);
    jg_class_add_method(&child_class, "overriden_method", &child_method);

    call_method(&child_class, "parent_method", "parent");
    call_method(&child_class, "child_method", "child");
    call_method(&parent_class, "overriden_method", "parent");
    call_method(&child_class, "overriden_method", "child");

    jg_class_cleanup(&child_class);
    jg_class_cleanup(&parent_class);
}

/** Registering two members with the same name should raise an assert. */
static void test_duplicated_member_fail(void** state) {
    (void)state;
    jg_class class_;

    jg_class_init(&class_, NULL, sizeof(char));
    jg_class_add_method(&class_, "member", &parent_method);

    expect_assert_failure(jg_class_add_method(&class_, "member", &child_method));
    expect_assert_failure(jg_class_add_property(&class_, "member", &child_getter, NULL));

    jg_class_cleanup(&class_);
}

void test_class(jg_vector* vector) {
    (void)vector;
    struct CMUnitTest class_tests[] = {
        cmocka_unit_test(test_add_property),
        cmocka_unit_test(test_add_method),
        cmocka_unit_test(test_duplicated_member_fail),
    };

    jg_vector_append(vector, class_tests, sizeof(class_tests) / sizeof(struct CMUnitTest));
}


