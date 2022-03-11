/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "jgui/private/object-model/module.h"

#include "jgui/private/containers/vector.h"
#include "jgui/private/object-model/class.h"

#include "tests/common/cmocka.h"

/** Classe should correctly be registered in modules. */
static void test_add_class(void** state) {
    (void)state;
    jg_module module;
    jg_module_init(&module);

    jg_class* class_ = jg_module_add_class(&module, "test_class", NULL, sizeof(char));
    assert_non_null(class_);

    const jg_class* retrieved_class = jg_module_get_class(&module, "test_class");

    assert(retrieved_class == class_);

    jg_module_cleanup(&module);
}

void test_module(jg_vector* vector) {
    (void)vector;
    struct CMUnitTest module_tests[] = {
        cmocka_unit_test(test_add_class),
    };

    jg_vector_append(vector, module_tests, sizeof(module_tests) / sizeof(struct CMUnitTest));
}



