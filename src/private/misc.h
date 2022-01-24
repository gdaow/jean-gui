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
#include <stddef.h>

size_t _ulg_strnlen(const char *s, size_t maxlen);


typedef int (*CompareFunction)(void* a, void* b);

typedef struct __Index {
    const char** keys;
    size_t count;
} _Index;

void _index_sort(_Index* index);
int _index_search(const _Index* index, const char* key);
