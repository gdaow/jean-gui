/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#ifndef JGUI_TESTS_COMMON_CMOCKA_H
#define JGUI_TESTS_COMMON_CMOCKA_H

#include <stdarg.h>
#include <stdint.h>
#include <setjmp.h>
#include <stddef.h>

// IWYU pragma: begin_exports
#include <cmocka.h>
// IWYU pragma: end_exports

#include "jgui/private/misc/assert.h"

#define jg_begin_tests(setup, teardown)\
int main (void) {\
    int (*_setup)(void**) = setup;\
    int (*_teardown)(void**) = teardown;\
    const struct CMUnitTest tests [] = {

#define jg_end_tests()\
    };\
    return cmocka_run_group_tests(tests, _setup, _teardown);\
}

#endif
