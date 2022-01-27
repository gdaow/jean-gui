/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <string.h>
#include <minunit.h>

#include <jg/object.h>
#include <jg/template.h>

#include "fixtures/user_model.h"


jg_module* module = NULL;

MU_TEST(test_jg_template_scalar_property) {
    jg_template* admin_template = jg_template_from_string(
        "!Admin\n"
        "name: Dr. Meeseeks\n"
        "team: !Team\n"
        "  name: Team Meeseeks\n",
        module
    );

    admin* admin = (struct admin_s*)jg_template_instanciate(admin_template);
    mu_assert_string_eq(admin->base.name, "Dr. Meeseeks");

    jg_object_free(admin);
    jg_template_free(admin_template); // teamemplate will be freed by i's parent
}

static void setup() {
    module = user_model_module_new();
}

static void teardown() {
    jg_module_free(module);
    module = NULL;
}

MU_TEST_SUITE(template_suite) {
    MU_SUITE_CONFIGURE(setup, teardown);
    MU_RUN_TEST(test_jg_template_scalar_property);
}
