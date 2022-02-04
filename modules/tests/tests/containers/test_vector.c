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

#define test_item_size 53
typedef const char test_item[test_item_size];
static const char test_item_1[53] = "Le Congrès considère que cette déclaration est une";
/*
static const char test_item_2[53] = "reconnaissance de la lutte de classe, qui oppose sur";
static const char test_item_3[53] = "le terrain économique, les travailleurs en révolte ";
static const char test_item_4[53] = "contre toutes les formes d'exploitation et ";
static const char test_item_5[53] = "d'oppression, tant matérielles que morales, mises ";
static const char test_item_6[53] = "en oeuvre par la classe capitaliste contre la classe";
static const char test_item_7[53] = "ouvrière.";
*/

void test_vector_size(void** state) {
    (void)state;
    jg_vector vector = jg_vector_of(test_item);
    assert_int_equal(jg_vector_size(&vector), 0);

    const size_t number_pushes = 64;
    for(size_t i = 0; i < number_pushes; ++i) {
        assert_int_equal(jg_vector_size(&vector), i);
        jg_vector_push(&vector, test_item_1);
    }

    jg_vector_cleanup(&vector);
}

void test_vector(jg_vector* vector) {
    struct CMUnitTest vector_tests[] = {
        cmocka_unit_test(test_vector_size),
    };
    jg_vector_append(vector, vector_tests, sizeof(vector_tests) / sizeof(struct CMUnitTest));
}
