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

void test_context(jg_vector* tests);
void test_id(jg_vector* tests);

void test_core(jg_vector* tests) {
    test_context(tests);
    test_id(tests);
}

