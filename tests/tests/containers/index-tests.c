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

#include "jgui/private/memory/alloc.h"

#include "common/cmocka.h"

#define test_key_size 256
#define test_item_count 9
typedef const char key[test_key_size];
static const key keys[test_item_count] = {"Apple",
                                          "Orange",
                                          "Peache",
                                          "Grape",
                                          "Watermelon",
                                          "Pear",
                                          "Mango",
                                          "T-34 85mm Soviet Tanks",
                                          ("Debout les damnés de la terre"
                                           "Debout les forçats de la faim"
                                           "La raison tonne en son cratère"
                                           "C'est l'éruption de la faim"
                                           "Du passé faisons table rase"
                                           "Foule esclave debout debout"
                                           "Le monde va changer de base"
                                           "Nous ne sommes rien soyons tout")};

static const int items[test_item_count] = {
    1,
    2,
    3,
    4,
    5,
    9000,
    -1,
};

static void check_index_values(jg_index* index) {
	assert_int_equal(NULL, jg_index_get(index, "Not In The Index"));

	for (int i = 0; i < index->_count; ++i) {
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

	for (size_t i = 0; i < test_item_count; ++i) {
		jg_index_add(&index, keys[i], &items[i]);
		check_index_values(&index);
	}

	jg_index_cleanup(&index, NULL);
}

static void add_and_pack(jg_index* index,
                         size_t start_item,
                         size_t end_item,
                         void** pack_buffer) {
	for (size_t i = start_item; i < end_item; ++i) {
		jg_index_add(index, keys[i], &items[i]);
	}
	check_index_values(index);

	*pack_buffer = jg_malloc(jg_index_packed_size(index));
	jg_index_pack(index, *pack_buffer);
	check_index_values(index);
}

/** We should get items in the index before and after packing it **/
static void test_index_build(void** state) {
	(void)state;

	jg_index index;
	void* pack_buffers[3] = {0};

	jg_index_init(&index, sizeof(int));

	pack_buffers[0] = jg_malloc(jg_index_packed_size(&index));
	jg_index_pack(&index, pack_buffers[0]); // test pack empty index

	add_and_pack(&index, 0, test_item_count / 2, &(pack_buffers[1]));
	add_and_pack(
	    &index, test_item_count / 2, test_item_count, &(pack_buffers[2]));

	jg_index_cleanup(&index, NULL);

	jg_free(pack_buffers[0]);
	jg_free(pack_buffers[1]);
	jg_free(pack_buffers[2]);
}

static int cleaned_item = 0;

static void cleanup_item(void* item) {
	cleaned_item = *((int*)item);
}

/** Item cleanup should be called when cleaning index. */
static void test_index_cleanup(void** state) {
	(void)state;
	jg_index index;

	jg_index_init(&index, sizeof(int));
	int test_item = 0xDEADBEEF; // NOLINT
	jg_index_add(&index, "Test", &test_item);

	cleaned_item = 0;
	jg_index_cleanup(&index, cleanup_item);
	assert_int_equal(cleaned_item, test_item);
}

jg_begin_tests(NULL, NULL)
	cmocka_unit_test(test_index_add_get), cmocka_unit_test(test_index_build),
	    cmocka_unit_test(test_index_cleanup),
jg_end_tests();
