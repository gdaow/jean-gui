/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include "jgui/private/containers/index.h"

#include <string.h>

#include "jgui/private/containers/vector.h"
#include "jgui/private/memory/alloc.h"

#include "tests/common/cmocka.h"

#define test_item_size 48
#define test_item_count 6
typedef const char key[test_item_size];
static const key keys[test_item_count] = {
    "Apples",
    "Oranges",
    "Peaches",
    "Grapes",
    "Watermelon",
    "T-34 85mm canon Soviet Tanks"
};

static const int items[test_item_count] = {
    1,
    2,
    3,
    4,
    5,
    9000
};


static void test_index_add(void** state) {
    (void)state;
}

static void test_index_build(void** state) {
    (void)state;
}

static void test_index_get(void** state) {
    (void)state;
}

static void test_index_item_cleanup(void** state) {
    (void)state;
}

static int setup(void **state) {
    jg_index* index = jg_malloc(sizeof(jg_index));
    jg_index_init(index, sizeof(int));
    for(size_t i = 0; i < test_item_count; ++i) {
        jg_index_add(index, keys[i], &items[i]);
    }
    *state = index;
    return 0;
}

static int teardown(void **state) {
    jg_index* index = *state;
    jg_index_cleanup(index, NULL);
    jg_free(index);
    return 0;
}

void test_index(jg_vector* vector) {
    (void)vector;
    struct CMUnitTest vector_tests[] = {
        cmocka_unit_test_setup_teardown(test_index_add, setup, teardown),
        cmocka_unit_test_setup_teardown(test_index_build, setup, teardown),
        cmocka_unit_test_setup_teardown(test_index_get, setup, teardown),
        cmocka_unit_test_setup_teardown(test_index_item_cleanup, setup, teardown),
    };

    jg_vector_append(vector, vector_tests, sizeof(vector_tests) / sizeof(struct CMUnitTest));
}


