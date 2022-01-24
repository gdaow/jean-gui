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
#ifndef UILIGI_SRC_PRIVATE_MISC_H
#define UILIGI_SRC_PRIVATE_MISC_H

#include <stddef.h>

size_t ulg_strnlen(const char *s, size_t max_size);

typedef struct ulg_index_s {
    const char** keys;
    size_t count;
} ulg_index_t;

void ulg_index_sort(ulg_index_t* index);
int ulg_index_search(const ulg_index_t* index, const char* key);

#endif
