/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <stdlib.h>
#include <string.h>

#include <jgui/core/context.h>
#include <jgui/object-model/object.h>

#include "jgui/private/misc/assert.h"
#include "fixtures/user_model.h"
#include "common.h"

/** Common fixture for all object tests. */
typedef struct fixture_s {
    jg_context* context;
    const jg_class* user_class;
    const jg_class* admin_class;
} fixture;

static int setup(void** state) {
    jg_context* context = jg_context_load((jg_plugin[]) {
        user_model_plugin,
        NULL
    });

    *state = malloc(sizeof(fixture));
    if(!*state) {
        return -1;
    }
    
    memcpy(
        *state,
        &(fixture) {
            .context = context,
            .user_class = jg_context_get_class(context, user_class_id()),
            .admin_class = jg_context_get_class(context, admin_class_id())
        },
        sizeof(fixture)
    );

    return 0;
}

static int teardown(void** state) {
    jg_context_free((*(fixture**)state)->context);
    return 0;
}

/** jg_class_get should return the class object given a class name and return NULL if the class was not
 *  registered in the module. */
static void test_jg_class_get(void** state) {
    fixture* fixture = *state;
    jg_context* context = fixture->context;

    const jg_class* user_class = jg_context_get_class(context, user_class_id());
    assert_non_null(user_class);

    const jg_class* admin_class = jg_context_get_class(context, admin_class_id());
    assert_non_null(admin_class);

    const jg_class* wrong_namespace = jg_context_get_class(
        context,
        jg_id_new("https://ki-dour.org/jg/dummy-ns", "Admin")
    );

    assert_null(wrong_namespace);
}

/*
static void test_constructors(const jg_class* class_, const char* expected_class_id) {
    user* user = jg_object_new(class_);

    mu_assert_string_eq(expected_class_id, user->constructed_class_id);

    const char* destroyed_class_id;
    mu_check(user->destructed_class_id == NULL);
    user->destructed_class_id = &destroyed_class_id;

    jg_object_free(user);
    mu_assert_string_eq(expected_class_id, destroyed_class_id);
}

MU_TEST(test_jg_object_constructor) {
    test_constructors(user_class, user_class_id);
    test_constructors(admin_class, admin_class_id);
}
*/

/** jg_object_get should correctly retrieve a property, be it declared on the type or on a parent. */
static void test_jg_object_get(void** state) {
    fixture* fixture = *state;
    admin* admin = jg_object_new(fixture->admin_class);
    const char* test_string = "Jean-jean mi";
    admin->base.name = test_string;
    admin->role = test_string;

    jg_value property;
    
    property = jg_object_get(admin, "name");
    assert_string_equal(test_string, jg_to_string(property));

    property = jg_object_get(admin, "role");
    assert_string_equal(test_string, jg_to_string(property));

    jg_object_free(admin);
}

/** jg_object_set should correctly set a property, be it declared on the type or on a parent. */
static void test_jg_object_set(void** state) {
    fixture* fixture = *state;

    admin* admin = jg_object_new(fixture->admin_class);
    const char* test_string = "Jean-jean mi";
    jg_value test_value = jg_string(test_string);

    jg_object_set(admin, "name", test_value);
    jg_object_set(admin, "role", test_value);
    assert_string_equal(test_string, admin->base.name);
    assert_string_equal(test_string, admin->role);

    jg_object_free(admin);
}

/** dynamic method calls shoud call the correct override */
static void test_jg_object_call(void** state) {
    fixture* fixture = *state;

    user* user = jg_object_new(fixture->user_class);
    admin* admin = jg_object_new(fixture->admin_class);
    
    assert(user_has_permission(user, PERM_LOGIN));
    assert(user_has_permission(user, PERM_CHANGE_PASSWORD));
    assert(!user_has_permission(user, PERM_CREATE_USER));
    assert(!user_has_permission(user, PERM_DELETE_USER));
    assert(user_has_permission(&admin->base, PERM_ALL));

    jg_object_free(admin);
    jg_object_free(user);
}

void run_object_suite() {
    const struct CMUnitTest test_object[] = {
        cmocka_unit_test(test_jg_class_get),
        //cmocka_unit_test(test_jg_object_constructor),
        cmocka_unit_test(test_jg_object_get),
        cmocka_unit_test(test_jg_object_set),
        cmocka_unit_test(test_jg_object_call)
    };

    cmocka_run_group_tests(test_object, setup, teardown);
}
