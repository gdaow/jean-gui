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
#ifndef MEZO_SRC_PRIVATE_MISC_H
#define MEZO_SRC_PRIVATE_MISC_H

#include <stddef.h>

size_t jg_strnlen(const char *s, size_t max_size);

typedef struct jg_index_s {
    const char** keys;
    size_t count;
} jg_index;

void jg_index_sort(jg_index* index);
int jg_index_search(const jg_index* index, const char* key);

#endif
