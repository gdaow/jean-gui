/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * TODO:
 *  - document.
 *
 */
#ifndef JGUI_PRIVATE_COMMON_INDEX_H
#define JGUI_PRIVATE_COMMON_INDEX_H

#include <stddef.h>

typedef struct jg_index_s {
    const char** keys;
    size_t count;
} jg_index;

jg_index jg_index_build(
    const void* first_item,
    const char* (*get_id_and_next)(const void** next_item),
    void (*build_item)(const void* item, int sorted_id, void* user_data),
    const char*** index_pool,
    char** string_pool,
    void* user_data
 );

int jg_index_search(const jg_index* index, const char* key);

#endif
