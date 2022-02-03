/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <string.h>

#include <jgui/object-model/object.h>
#include <jgui/template.h>

#include "fixtures/user_model.h"
#include "common.h"


void test_jg_template_scalar_property(void** state) {
    jg_context* context = *state;
    jg_template* admin_template = jg_template_from_string(
        "%TAG ! http://ki-dour.org/jean-gui/tests/user-model:\n"
        "---\n"
        "!Admin\n"
        "name: Dr. Meeseeks\n"
        "team: !Team\n"
        "  name: Team Meeseeks\n",
        context
    );

    admin* admin = (struct admin_s*)jg_template_instanciate(admin_template);
    assert_string_equal(admin->base.name, "Dr. Meeseeks");

    jg_object_free(admin);
    jg_template_free(admin_template); // teamemplate will be freed by i's parent
}

static int setup(void** state) {
    *state = jg_context_load((jg_plugin[]) {
        user_model_plugin,
        NULL
    });
    
    if(!*state) {
        return -1;
    }

    return 0;
}

static int teardown(void** state) {
    jg_context_free(*state);
    return 0;
}

void run_template_suite() {
    const struct CMUnitTest template_tests[] = {
        cmocka_unit_test(test_jg_template_scalar_property),
    };

    cmocka_run_group_tests(template_tests, setup, teardown);
}
