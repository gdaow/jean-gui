/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include "jgui/private/containers/vector.h"

#include "tests/common/cmocka.h"

#define test_item_size 54
typedef const char item[test_item_size];
static const item items[7] = {
    "Le Congrès considère que cette déclaration est une\0",
    "reconnaissance de la lutte de classe, qui oppose sur\0",
    "le terrain économique, les travailleurs en révolte \0",
    "contre toutes les formes d'exploitation et \0",
    "d'oppression, tant matérielles que morales, mises \0",
    "en oeuvre par la classe capitaliste contre la classe\0",
    "ouvrière."
};

static const size_t item_count = sizeof(items) / sizeof(item);

void test_vector_push(void** state) {
    (void)state;
    jg_vector vector = jg_vector_of(item);
    assert_int_equal(jg_vector_size(&vector), 0);

    for(size_t i = 0; i < item_count; ++i) {
        jg_vector_push(&vector, items[i]);
        assert_int_equal(jg_vector_size(&vector), i + 1);
        assert_string_equal(jg_vector_back(&vector), items[i]);
    }

    jg_vector_cleanup(&vector);
}

void test_vector_size(void** state) {
    (void)state;
    jg_vector vector = jg_vector_of(item);
    assert_int_equal(jg_vector_size(&vector), 0);

    const size_t number_pushes = 64;
    for(size_t i = 0; i < number_pushes; ++i) {
        assert_int_equal(jg_vector_size(&vector), i);
        jg_vector_push(&vector, items[0]);
    }

    jg_vector_cleanup(&vector);
}

void test_vector(jg_vector* vector) {
    struct CMUnitTest vector_tests[] = {
        cmocka_unit_test(test_vector_push),
        cmocka_unit_test(test_vector_size),
    };
    jg_vector_append(vector, vector_tests, sizeof(vector_tests) / sizeof(struct CMUnitTest));
}
