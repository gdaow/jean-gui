/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <string.h>
#include <minunit.h>

#include <uiligi/object.h>
#include <uiligi/template.h>

#include "fixtures/user_model.h"


ulg_module_t* module = NULL;

MU_TEST(test_ulg_template_scalar_property) {
    ulg_template_t* admin_template = ulg_template_from_string(
        "<Admin name=\"Dr. Meeseeks\" />",
        module
    );

    admin_t* admin = (admin_t*)ulg_template_instanciate(admin_template);
    mu_assert_string_eq(admin->base.name, "Dr. Meeseeks");

    ulg_object_free(admin);
    ulg_template_free(admin_template); // team_template will be freed by i's parent
    ulg_module_free(module);
}

static void setup() {
    module = user_model_module_new();
}

static void teardown() {
    ulg_module_free(module);
    module = NULL;
}

MU_TEST_SUITE(template_suite) {
    MU_SUITE_CONFIGURE(setup, teardown);
    MU_RUN_TEST(test_ulg_template_scalar_property);
}
