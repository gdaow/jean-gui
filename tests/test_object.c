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

#include <uiligi/module.h>
#include <uiligi/object.h>

#include "fixtures/user_model.h"

static UlgModule* module = NULL;
static const UlgClass* user_class = NULL;
static const UlgClass* admin_class = NULL;

void _dummy_type(UlgClassDefinition* class_) {
    (void)class_;
}

/** ulg_class_get should return the class object given a class name or a class definition, and return
 *  NULL if the class was not registered in the module. */
MU_TEST(test_ulg_class_get) {
    const UlgClass* user_class = ulg_class_get_by_name(module, "User");
    mu_check(user_class != NULL);
    mu_check(user_class == ulg_class_get(module, user_type));

    const UlgClass* admin_class = ulg_class_get_by_name(module, "Admin");
    mu_check(admin_class != NULL);
    mu_check(admin_class == ulg_class_get(module, admin_type));

    mu_check(ulg_class_get(module, _dummy_type) == NULL);
    mu_check(ulg_class_get_by_name(module, "Dummy") == NULL);
}

/** ulg_object_get should correctly retrieve a property, be it declared on the type or on a parent. */
MU_TEST(test_ulg_object_get) {
    Admin* admin = ulg_object_new(admin_class);
    const char* test_string = "Jean-jean mi";
    admin->base.name = test_string;
    admin->role = test_string;

    UlgValue property;
    
    property = ulg_object_get(admin, "name");
    mu_assert_string_eq(test_string, ulg_to_string(property));

    property = ulg_object_get(admin, "role");
    mu_assert_string_eq(test_string, ulg_to_string(property));

    ulg_object_free(admin);
}

/** ulg_object_set should correctly set a property, be it declared on the type or on a parent. */
MU_TEST(test_ulg_object_set) {
    Admin* admin = ulg_object_new(admin_class);
    const char* test_string = "Jean-jean mi";
    UlgValue test_value = ulg_string(test_string);

    ulg_object_set(admin, "name", test_value);
    ulg_object_set(admin, "role", test_value);
    mu_assert_string_eq(test_string, admin->base.name);
    mu_assert_string_eq(test_string, admin->role);

    ulg_object_free(admin);
}

static void _setup() {
    module = user_model_module_new();
    user_class = ulg_class_get(module, user_type);
    admin_class = ulg_class_get(module, admin_type);
}

static void _teardown() {
    admin_class = NULL;
    user_class = NULL;
    ulg_module_free(module);
    module = NULL;
}

MU_TEST_SUITE(object_suite) {
    MU_SUITE_CONFIGURE(_setup, _teardown);
    MU_RUN_TEST(test_ulg_class_get);
    MU_RUN_TEST(test_ulg_object_get);
    MU_RUN_TEST(test_ulg_object_set);
}
