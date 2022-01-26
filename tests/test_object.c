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

#include <mezo/object.h>

#include "fixtures/user_model.h"

static mz_module* module = NULL;
static const mz_class* user_class = NULL;
static const mz_class* admin_class = NULL;

void _dummy_type(mz_class_definition* class_) {
    (void)class_;
}

/** mz_class_get should return the class object given a class name and return NULL if the class was not
 *  registered in the module. */
MU_TEST(test_mz_class_get) {
    const mz_class* user_class = mz_class_get(module, USER);
    mu_check(user_class != NULL);

    const mz_class* admin_class = mz_class_get(module, ADMIN);
    mu_check(admin_class != NULL);

    mu_check(mz_class_get(module, "Dummy") == NULL);
}

/** mz_object_get should correctly retrieve a property, be it declared on the type or on a parent. */
MU_TEST(test_mz_object_get) {
    admin_t* admin = mz_object_new(admin_class);
    const char* test_string = "Jean-jean mi";
    admin->base.name = test_string;
    admin->role = test_string;

    mz_value property;
    
    property = mz_object_get(admin, "name");
    mu_assert_string_eq(test_string, mz_to_string(property));

    property = mz_object_get(admin, "role");
    mu_assert_string_eq(test_string, mz_to_string(property));

    mz_object_free(admin);
}

/** mz_object_set should correctly set a property, be it declared on the type or on a parent. */
MU_TEST(test_mz_object_set) {
    admin_t* admin = mz_object_new(admin_class);
    const char* test_string = "Jean-jean mi";
    mz_value test_value = mz_string(test_string);

    mz_object_set(admin, "name", test_value);
    mz_object_set(admin, "role", test_value);
    mu_assert_string_eq(test_string, admin->base.name);
    mu_assert_string_eq(test_string, admin->role);

    mz_object_free(admin);
}

static void setup() {
    module = user_model_module_new();
    user_class = mz_class_get(module, USER);
    admin_class = mz_class_get(module, ADMIN);
}

static void teardown() {
    admin_class = NULL;
    user_class = NULL;
    mz_module_free(module);
    module = NULL;
}

MU_TEST_SUITE(object_suite) {
    MU_SUITE_CONFIGURE(setup, teardown);
    MU_RUN_TEST(test_mz_class_get);
    MU_RUN_TEST(test_mz_object_get);
    MU_RUN_TEST(test_mz_object_set);
}
