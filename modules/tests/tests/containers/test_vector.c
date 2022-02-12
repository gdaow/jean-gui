/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include "jgui/private/containers/vector.h"

#include <string.h>

#include "jgui/private/memory/alloc.h"

#include "tests/common/cmocka.h"

#define test_item_size 54
typedef const char item[test_item_size];
static const item items[7] = {
    "Le Congrès considère que cette déclaration est une",
    "reconnaissance de la lutte de classe, qui oppose sur",
    "le terrain économique, les travailleurs en révolte ",
    "contre toutes les formes d'exploitation et ",
    "d'oppression, tant matérielles que morales, mises ",
    "en oeuvre par la classe capitaliste contre la classe",
    "ouvrière."
};

static const size_t item_count = sizeof(items) / sizeof(item);

static void test_vector_push(void** state) {
    jg_vector* vector = *state;

    for(size_t i = 0; i < 1; ++i) {
        jg_vector_push(vector, items[i]);
        assert_int_equal(jg_vector_size(vector), i + 1);
        assert_string_equal(jg_vector_back(vector), items[i]);
    }
}

void test_vector_append(void** state) {
    jg_vector* vector = *state;

    jg_vector_append(vector, items, item_count);

    for(size_t i = 0; i < item_count; ++i) {
        assert_string_equal(jg_vector_at(vector, i), items[i]);
    }
}

void test_vector_front_at_back(void** state) {
    (void)state;
}


void test_vector_size(void** state) {
    jg_vector* vector = *state;
    assert_int_equal(jg_vector_size(vector), 0);

    const size_t number_pushes = 64;
    for(size_t i = 0; i < number_pushes; ++i) {
        assert_int_equal(jg_vector_size(vector), i);
        jg_vector_push(vector, items[0]);
    }
}

static int setup(void **state) {
    jg_vector* vector = jg_malloc(sizeof(jg_vector));
    if (vector == NULL) {
        return -1;
    }
    jg_vector init_vector = jg_vector_of(item);
    memcpy(vector, &init_vector, sizeof(jg_vector));
    *state = vector;
    return 0;
}

static int teardown(void **state) {
    jg_vector_cleanup(*state);
    jg_free(*state);
    return 0;
}

void test_vector(jg_vector* vector) {
    (void)vector;
    struct CMUnitTest vector_tests[] = {
        cmocka_unit_test_setup_teardown(test_vector_push, setup, teardown),
    };
    cmocka_run_group_tests(vector_tests, NULL, NULL);
}
