/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include <jgui/core/context.h>

#include "jgui/private/containers/vector.h"

#include "tests/common/cmocka.h"

/** jg_id_is_none should return true only for JG_ID_NONE. */
static void test_jg_id_none(void** state) {
    (void)state;
    assert(jg_id_is_null(JG_ID_NULL));
    assert(!jg_id_is_null(JG_ID("namespace", "name")));
}

void test_id(jg_vector* vector) {
    (void)vector;
    struct CMUnitTest id_tests[] = {
        cmocka_unit_test(test_jg_id_none),
    };

    jg_vector_append(vector, id_tests, sizeof(id_tests) / sizeof(struct CMUnitTest));
}

