/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <minunit.h>

#include <jgui/module.h>
#include <jgui/object.h>

#include "fixtures/user_model.h"

static jg_module* module = NULL;
static const jg_class* user_class = NULL;
static const jg_class* admin_class = NULL;

void _dummy_type(jg_class_definition* class_) {
    (void)class_;
}

/** jg_class_get should return the class object given a class name and return NULL if the class was not
 *  registered in the module. */
MU_TEST(test_jg_class_get) {
    const jg_class* user_class = jg_module_get_class(module, user_class_id);
    mu_check(user_class != NULL);

    const jg_class* admin_class = jg_module_get_class(module, admin_class_id);
    mu_check(admin_class != NULL);

    mu_check(jg_module_get_class(module, "Dummy") == NULL);
}

static void test_constructors(const jg_class* class_, const char* expected_class_id) {
    user* user = jg_object_new(class_);

    mu_assert_string_eq(user->constructed_class_id, expected_class_id);

    const char* destroyed_class_id;
    mu_check(user->destructed_class_id == NULL);
    user->destructed_class_id = &destroyed_class_id;

    jg_object_free(user);
    mu_assert_string_eq(destroyed_class_id, expected_class_id);
}

/** jg_object_get should correctly retrieve a property, be it declared on the type or on a parent. */
MU_TEST(test_jg_object_constructor) {
    test_constructors(user_class, user_class_id);
    test_constructors(admin_class, admin_class_id);
}

/** jg_object_get should correctly retrieve a property, be it declared on the type or on a parent. */
MU_TEST(test_jg_object_get) {
    admin* admin = jg_object_new(admin_class);
    const char* test_string = "Jean-jean mi";
    admin->base.name = test_string;
    admin->role = test_string;

    jg_value property;
    
    property = jg_object_get(admin, "name");
    mu_assert_string_eq(test_string, jg_to_string(property));

    property = jg_object_get(admin, "role");
    mu_assert_string_eq(test_string, jg_to_string(property));

    jg_object_free(admin);
}

/** jg_object_set should correctly set a property, be it declared on the type or on a parent. */
MU_TEST(test_jg_object_set) {
    admin* admin = jg_object_new(admin_class);
    const char* test_string = "Jean-jean mi";
    jg_value test_value = jg_string(test_string);

    jg_object_set(admin, "name", test_value);
    jg_object_set(admin, "role", test_value);
    mu_assert_string_eq(test_string, admin->base.name);
    mu_assert_string_eq(test_string, admin->role);

    jg_object_free(admin);
}

/** dynamic method calls shoud call the correct override */
MU_TEST(test_jg_object_call) {
    user* user = jg_object_new(user_class);
    admin* admin = jg_object_new(admin_class);
    
    mu_check(user_has_permission(user, PERM_LOGIN));
    mu_check(user_has_permission(user, PERM_CHANGE_PASSWORD));
    mu_check(!user_has_permission(user, PERM_CREATE_USER));
    mu_check(!user_has_permission(user, PERM_DELETE_USER));
    mu_check(user_has_permission(&admin->base, PERM_ALL));

    jg_object_free(admin);
    jg_object_free(user);
}

static void setup() {
    module = user_model_module_new();
    user_class = jg_module_get_class(module, user_class_id);
    admin_class = jg_module_get_class(module, admin_class_id);
}

static void teardown() {
    admin_class = NULL;
    user_class = NULL;
    jg_module_free(module);
    module = NULL;
}

MU_TEST_SUITE(object_suite) {
    MU_SUITE_CONFIGURE(setup, teardown);
    MU_RUN_TEST(test_jg_class_get);
    MU_RUN_TEST(test_jg_object_constructor);
    MU_RUN_TEST(test_jg_object_get);
    MU_RUN_TEST(test_jg_object_set);
    MU_RUN_TEST(test_jg_object_call);
}
