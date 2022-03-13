/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include "jgui/private/memory/string.h"

#include <string.h>

#include "jgui/private/misc/assert.h"

size_t jg_strnlen(const char *s, size_t max_size) {
    assert(s != NULL);

    const char* end = memchr(s, '\0', max_size);
    if (end == NULL) {
        return max_size;
    }

    assert(end >= s);
    return (size_t)(end - s);
}

bool jg_null_or_empty(const char* str) {
    return str == NULL || *str == '\0';
}
