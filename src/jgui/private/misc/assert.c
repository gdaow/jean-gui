/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include "jgui/private/misc/assert.h"

#include <stdio.h>
#include <stdlib.h>

#include <jgui/static-config.h>

#if JG_ENABLE_ASSERTS

void jg_assert_impl(bool test_result, const char* test_expression, const char* file, int line) {
    if(!test_result) {
        fprintf(stderr, "JEAN-GUI ASSERT FAILED:(%s:%i):%s", file, line, test_expression);
        abort();
    }
}

#endif
