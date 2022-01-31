/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include <stdio.h>
#include <stdlib.h>

#include <jgui/debug.h>

#if JG_DEBUG

static void default_assert_handler(const char* expression, const char* file, int line);

static jg_assert_handler assert_handler = default_assert_handler;

void jg_set_assert_handler(jg_assert_handler handler) {
    assert_handler = handler;
}

void jg_assert(bool test, const char* expression, const char* file, int line) {
    if(!test) {
        assert_handler(expression, file, line);
    }
}

static void default_assert_handler(const char* expression, const char* file, int line) {
    fprintf(stderr, "JEAN-GUI ASSERT FAILED:(%s:%i):%s", file, line, expression);
    abort();
}

#endif
