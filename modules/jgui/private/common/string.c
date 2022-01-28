/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include <string.h>

#include "string.h"
#include "debug.h"

size_t jg_strnlen(const char *s, size_t max_size) {
    JG_ASSERT(s != NULL);

    const char* end = memchr(s, '\0', max_size);
    if (end == NULL) {
        return max_size;
    }

    JG_ASSERT(end >= s);
    return (size_t)(end - s);
}
