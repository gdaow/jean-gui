/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * Miscelaneous helpers & utilities.
 *
 */
#ifndef JEAN_GUI_SRC_PRIVATE_MISC_H
#define JEAN_GUI_SRC_PRIVATE_MISC_H

#include <stdbool.h>
#include <stddef.h>

#ifndef NDEBUG
void jg_assert_handler(
    bool test,
    const char* test_string,
    const char* file,
    int line
);
#   define JG_ASSERT(TEST)\
        (jg_assert_handler(TEST, #TEST, __FILE__, __LINE__))
#else
#   define JG_ASSERT(message, ...)
#endif

size_t jg_strnlen(const char *s, size_t max_size);

typedef struct jg_index_s {
    const char** keys;
    size_t count;
} jg_index;

void jg_index_sort(jg_index* index);
int jg_index_search(const jg_index* index, const char* key);

#endif
