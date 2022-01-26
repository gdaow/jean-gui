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

size_t mz_strnlen(const char *s, size_t max_size);

typedef struct mz_index_s {
    const char** keys;
    size_t count;
} mz_index;

void mz_index_sort(mz_index* index);
int mz_index_search(const mz_index* index, const char* key);

#endif
