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

MU_TEST(test_ulg_template_instanciate) {
    ulg_module_t* module = user_model_module_new();

    const char* name = "Dr. Meeseeks";
    const char* role = "Manager of all the Mr Meeseeks";
    const char* team_name = "Team Meeseeks";

    ulg_template_t* admin_template = ulg_template_new_by_name(module, ADMIN);

    ulg_template_set_scalar(admin_template, "name", name);
    ulg_template_set_scalar(admin_template, "role", role);

    //TODO: Handle default type for children here.
    ulg_template_t* team_template = ulg_template_set_child(admin_template, "team", TEAM);
    ulg_template_set_scalar(team_template, "name", team_name);

    admin_t* admin = (admin_t*)ulg_template_instanciate(admin_template);
    mu_assert_string_eq(admin->base.name, name);
    mu_assert_string_eq(admin->role, role);
    mu_assert_string_eq(admin->base.team->name, team_name);

    ulg_object_free(admin->base.team); //TODO: We should handle this either in admin_t, either automatically.
    ulg_object_free(admin);
    ulg_template_free(admin_template); // team_template will be freed by i's parent
    ulg_module_free(module);
}

MU_TEST_SUITE(template_suite) {
    MU_RUN_TEST(test_ulg_template_instanciate);
}
