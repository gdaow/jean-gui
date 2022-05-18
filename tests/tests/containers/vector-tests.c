/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include "jgui/private/containers/vector.h"

#include <string.h>
#include <stdlib.h>

#include "jgui/private/memory/alloc.h"

#include "common/cmocka.h"

#define test_item_size 54
typedef const char item[test_item_size];
static const item items[7] = {
    "Le Congrès considère que cette déclaration est une",
    "reconnaissance de la lutte de classe, qui oppose sur",
    "le terrain économique, les travailleurs en révolte ",
    "contre toutes les formes d'exploitation et ",
    "d'oppression, tant matérielles que morales, mises ",
    "en oeuvre par la classe capitaliste contre la classe",
    "ouvrière."};

static const size_t item_count = sizeof(items) / sizeof(item);

/* Items should correctly be pushed in a vector */
static void test_vector_push(void** state) {
	jg_vector* vector = *state;

    {
        for (size_t i = 0; i < item_count; ++i) {
            jg_vector_push(vector, items[i]);
            assert_int_equal(jg_vector_size(vector), i + 1);
            assert_string_equal(jg_vector_back(vector), items[i]);
        }
        jg_vector_cleanup(vector);
    }
}

/* Batch of items should be added to the vector. */
void test_vector_append(void** state) {
	jg_vector* vector = *state;

	jg_vector_append(vector, items, item_count);

	for (size_t i = 0; i < item_count; ++i) {
		assert_string_equal(jg_vector_at(vector, i), items[i]);
	}
	jg_vector_cleanup(vector);
}

/* jg_vector_front, jg_vector_at and jg_vector_end should return correct
 * elements. */
void test_vector_front_at_back(void** state) {
	jg_vector* vector = *state;

	jg_vector_push(vector, items[0]);
	jg_vector_push(vector, items[1]);
	jg_vector_push(vector, items[2]);

	assert_string_equal(jg_vector_front(vector), items[0]);
	assert_string_equal(jg_vector_at(vector, 1), items[1]);
	assert_string_equal(jg_vector_back(vector), items[2]);

	jg_vector_cleanup(vector);
}

/* jg_vector_size should return the size of the vector. */
void test_vector_size(void** state) {
	jg_vector* vector = *state;
	assert_int_equal(jg_vector_size(vector), 0);

	const size_t number_pushes = 64;
	for (size_t i = 0; i < number_pushes; ++i) {
		assert_int_equal(jg_vector_size(vector), i);
		jg_vector_push(vector, items[0]);
	}
	jg_vector_cleanup(vector);
}

static int setup(void** state) {
	jg_vector* vector = jg_malloc(sizeof(jg_vector));
	jg_vector init_vector = jg_vector_of(item);
	memcpy(vector, &init_vector, sizeof(jg_vector));
	*state = vector;
	return 0;
}

static int teardown(void** state) {
	jg_free(*state);
	return 0;
}

jg_begin_tests(NULL, NULL)
	cmocka_unit_test_setup_teardown(test_vector_push, setup, teardown),
	    cmocka_unit_test_setup_teardown(test_vector_append, setup, teardown),
	    cmocka_unit_test_setup_teardown(
	        test_vector_front_at_back, setup, teardown),
	    cmocka_unit_test_setup_teardown(test_vector_size, setup, teardown),
jg_end_tests()
