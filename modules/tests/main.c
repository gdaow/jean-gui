/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include "jgui/private/containers/vector.h"

#include "tests/common/cmocka.h"

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
    mock_assert(test_result, test_expression, file, line);
}

void test_containers(jg_vector* tests);

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    jg_vector tests = jg_vector_of(struct CMUnitTest);
    test_containers(&tests);
    return _cmocka_run_group_tests(
        "JGui Tests",
        jg_vector_front(&tests),
        jg_vector_size(&tests),
        NULL,
        NULL
    );
}
