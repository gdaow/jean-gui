/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include <jgui/misc/utils.h>

jg_forward_declare(jg_vector)

void test_arguments(jg_vector* tests);
void test_class(jg_vector* tests);
void test_member(jg_vector* tests);
void test_module(jg_vector* tests);
void test_value(jg_vector* tests);

void test_object_model(jg_vector* tests) {
    test_arguments(tests);
    test_class(tests);
    test_member(tests);
    test_module(tests);
    test_value(tests);
}

