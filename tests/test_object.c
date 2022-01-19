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

#include <uiligi/object.h>

#include "fixtures/user_model.h"

static UlgContext* context = NULL;
static const UlgClass* user_class = NULL;
static const UlgClass* admin_class = NULL;

const UlgClass* _dummy_type(UlgClassFactory* factory) {
    (void)factory;
    return NULL;
}

/** ulg_class_get should return the class object given a class name or a class definition, and return
 *  NULL if the class was not registered in the context. */
MU_TEST(test_ulg_class_get) {
    const UlgClass* user_class = ulg_class_get_by_name(context, "User");
    mu_check(user_class != NULL);
    mu_check(user_class == ulg_class_get(context, user_type));

    const UlgClass* admin_class = ulg_class_get_by_name(context, "Admin");
    mu_check(admin_class != NULL);
    mu_check(admin_class == ulg_class_get(context, admin_type));

    mu_check(ulg_class_get(context, _dummy_type) == NULL);
    mu_check(ulg_class_get_by_name(context, "Dummy") == NULL);
}

/** ulg_object_vtable should allow override virtual functions of parent classes. */
MU_TEST(test_ulg_object_vtable) {
    User* user = ulg_object_new(user_class);
    User* admin = ulg_object_new(admin_class);

    mu_assert_int_eq(user_get_default_permissions(user), PERM_CAN_LOGIN);
    mu_assert_int_eq(user_get_default_permissions(admin), PERM_ALL);

    ulg_object_free(admin);
    ulg_object_free(user);
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
    context = user_model_context_new();
    user_class = ulg_class_get(context, user_type);
    admin_class = ulg_class_get(context, admin_type);
}

static void _teardown() {
    admin_class = NULL;
    user_class = NULL;
    ulg_context_free(context);
    context = NULL;
}

MU_TEST_SUITE(object_suite) {
    MU_SUITE_CONFIGURE(_setup, _teardown);
    MU_RUN_TEST(test_ulg_class_get);
    MU_RUN_TEST(test_ulg_object_vtable);
    MU_RUN_TEST(test_ulg_object_get);
    MU_RUN_TEST(test_ulg_object_set);
}
