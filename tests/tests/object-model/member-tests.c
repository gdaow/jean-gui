/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "jgui/private/object-model/member.h"

#include <stddef.h>

#include "jgui/object-model/arguments.h"
#include "jgui/object-model/value.h"

#include "common/cmocka.h"
#include "common/fixtures/user_model.h"

static jg_value user_name_getter(const void* object) {
	return jg_string(user_get_name(object));
}

static void user_name_setter(void* object, jg_value value) {
	user_set_name(object, jg_to_string(value));
}

/** jg_property_get should retrieve the object's property value */
static void test_property_get(void** state) {
	(void)state;
	jg_member member;
	test_user user = (test_user) {.name = "Henri"};

	jg_property_init(&member, user_name_getter, user_name_setter);

	jg_value property_value = jg_property_get(&member, &user);

	assert_string_equal(user.name, jg_to_string(property_value));

	jg_arguments arguments = jg_arguments_new(jg_none());
	expect_assert_failure(jg_method_call(&member, &arguments));
}

/** jg_property_set should set the object's property value */
static void test_property_set(void** state) {
	(void)state;
	jg_member member;
	test_user user = (test_user) {.name = "Georges"};

	jg_property_init(&member, user_name_getter, user_name_setter);

	jg_property_set(&member, &user, jg_string("Krascuky"));

	assert_string_equal(user.name, "Krascuky");

	jg_arguments arguments = jg_arguments_new(jg_none());
	expect_assert_failure(jg_method_call(&member, &arguments));
}

static jg_value method_fixture(jg_arguments* arguments) {
	assert_string_equal(jg_pop_string(arguments), "Henri");
	return jg_string("Krascuky");
}

/** jg_method_call should call the wrapped function pointer */
static void test_method_call(void** state) {
	(void)state;
	jg_member member;
	jg_method_init(&member, method_fixture);

	jg_arguments args = jg_arguments_new(jg_string("Henri"));
	jg_value result = jg_method_call(&member, &args);

	assert_string_equal(jg_to_string(result), "Krascuky");

	expect_assert_failure(jg_property_get(&member, &(test_user) {}));
}

jg_begin_tests(NULL, NULL)
	cmocka_unit_test(test_property_get), cmocka_unit_test(test_property_set),
	    cmocka_unit_test(test_method_call),
jg_end_tests()
