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
#define test_item_count 8
typedef const char key[test_item_size];
static const key keys[test_item_count] = {
    "Apple",
    "Orange",
    "Peache",
    "Grape",
    "Watermelon",
    "Pear",
    "Mango",
    "T-34 85mm Soviet Tanks"
};

static const int items[test_item_count] = {
    1,
    2,
    3,
    4,
    5,
    9000
};

static void check_index_values(jg_index* index) {
    assert_int_equal(NULL, jg_index_get(index, "Not In The Index"));

    for(int i = 0; i < index->count; ++i) {
        const char* key = keys[i];
        const int* item = jg_index_get(index, key);
        assert_int_equal(items[i], *item);
    }
}

/** We should get items in the index before and after adding them **/
static void test_index_add_get(void** state) {
    (void)state;
    jg_index index;

    jg_index_init(&index, sizeof(int));
    check_index_values(&index);

    for(size_t i = 0; i < test_item_count; ++i) {
        jg_index_add(&index, keys[i], &items[i]);
        check_index_values(&index);
    }

    jg_index_cleanup(&index, NULL);
}

/** We should get items in the index before and after building it **/
static void test_index_build(void** state) {
    (void)state;
    jg_index index;

    jg_index_init(&index, sizeof(int));

    for(size_t i = 0; i < test_item_count / 2; ++i) {
        jg_index_add(&index, keys[i], &items[i]);
    }

    jg_index_build(&index);
    check_index_values(&index);

    for(size_t i = test_item_count / 2; i < test_item_count; ++i) {
        jg_index_add(&index, keys[i], &items[i]);
    }

    check_index_values(&index);
    jg_index_build(&index);
    check_index_values(&index);
    jg_index_cleanup(&index, NULL);
}

void test_index(jg_vector* vector) {
    (void)vector;
    struct CMUnitTest vector_tests[] = {
        cmocka_unit_test_setup_teardown(test_index_add_get, NULL, NULL),
        cmocka_unit_test_setup_teardown(test_index_build, NULL, NULL),
    };

    jg_vector_append(vector, vector_tests, sizeof(vector_tests) / sizeof(struct CMUnitTest));
}


