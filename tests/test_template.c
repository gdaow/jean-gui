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

MU_TEST(template_instanciate) {
    UlgContext* context = user_model_context_new();

    UlgTemplate* template = ulg_template_new(context, "Admin");

    const char* name = "uber jean-mi";
    const char* role = "manager of all the jean-mi's";
    ulg_template_set_scalar(template, "name", name);
    ulg_template_set_scalar(template, "role", role);

    Admin* admin = (Admin*)ulg_template_instanciate(template);
    mu_assert_string_eq(user_get_name((User *)admin), name);
    mu_assert_string_eq(admin_get_role(admin), role);
    ulg_object_free((UlgObject*)admin);
    ulg_template_free(template);
    ulg_context_free(context);
}

MU_TEST_SUITE(template_suite) {
    MU_RUN_TEST(template_instanciate);
}
