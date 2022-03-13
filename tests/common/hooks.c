/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <stdbool.h>
#include <stdio.h>

#include "jgui/private/misc/assert.h"

#include "common/cmocka.h"

void* jg_malloc_impl(size_t size, const char* file, int line) {
    return _test_malloc(size, file, line);
}

void* jg_realloc_impl(void* ptr, size_t size, const char* file, int line) {
    return _test_realloc(ptr, size, file, line);
}

void* jg_calloc_impl(size_t nb_elt, size_t size, const char* file, int line) {
    return _test_calloc(nb_elt, size, file, line);
}

void jg_free_impl(void* ptr, const char* file, int line) {
    _test_free(ptr, file, line);
}

void jg_assert_impl(bool test_result, const char* test_expression, const char* file, int line) {
    if(!test_result && !global_expecting_assert) {
        fprintf(stderr, "Assertion failed (%s:%i): %s\n", file, line, test_expression);
    }
    
    mock_assert(test_result, test_expression, file, line);
}

