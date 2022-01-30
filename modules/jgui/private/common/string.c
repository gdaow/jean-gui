/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include <stdalign.h>
#include <string.h>

#include "debug.h"
#include "memory.h"
#include "string.h"

size_t jg_strnlen(const char *s, size_t max_size) {
    JG_ASSERT(s != NULL);

    const char* end = memchr(s, '\0', max_size);
    if (end == NULL) {
        return max_size;
    }

    JG_ASSERT(end >= s);
    return (size_t)(end - s);
}

char* jg_copy_string(jg_allocator* allocator, const char* value, size_t max_length) {
    JG_ASSERT(allocator != NULL);
    JG_ASSERT(value != NULL);

    size_t length = jg_strnlen(value, max_length) + 1;
    char* copy = jg_allocate_aligned(allocator, length, alignof(char));
    JG_ASSERT(copy != NULL); //TODO(corentin@ki-dour.org) handle error.
    memcpy(copy, value, length);
    copy[length] = '\0';
    return copy;
}

char* jg_copy_identifier(jg_allocator* allocator, const char* value) {
    JG_ASSERT(allocator != NULL);
    JG_ASSERT(value != NULL);

    return jg_copy_string(allocator, value, JG_MAX_IDENTIFIER_LENGTH);
}
