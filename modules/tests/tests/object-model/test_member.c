/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include <jgui/object-model/member.h>

#include "jgui/private/containers/vector.h"
#include "jgui/private/object-model/member.h"
#include "tests/common/fixtures/user_model.h"

#include "tests/common/cmocka.h"

static jg_value user_get_name(const void* object) {
    const user* user = object;
    return jg_string(user->name);
}

static void user_set_name(void* object, jg_value value) {
    user* user = object;
    user->name = jg_to_string(value);
}

static void test_property_get(void** state) {
    (void)state;
    jg_member member;
    user test_user = (user) {
        .name = "Henri"
    };

    jg_property_init(&member, user_get_name, user_set_name);
    jg_value property_value = jg_property_get(&test_user, &member);

    assert_string_equal(test_user.name, jg_to_string(property_value));
}

static void test_property_set(void** state) {
    (void)state;
    jg_member member;
    user test_user = (user) {
        .name = "Georges"
    };

    jg_property_init(&member, user_get_name, user_set_name);
    jg_property_set(&test_user, &member, jg_string("Krascuky"));

    assert_string_equal(test_user.name, "Krascuky");
}

void test_member(jg_vector* vector) {
    (void)vector;
    struct CMUnitTest member_tests[] = {
        cmocka_unit_test(test_property_get),
        cmocka_unit_test(test_property_set),
    };

    jg_vector_append(vector, member_tests, sizeof(member_tests) / sizeof(struct CMUnitTest));
}

