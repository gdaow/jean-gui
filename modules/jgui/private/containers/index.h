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
#ifndef JGUI_PRIVATE_CONTAINERS_INDEX_H
#define JGUI_PRIVATE_CONTAINERS_INDEX_H

#include <stddef.h>

#include <jgui/misc/utils.h>

jg_forward_declare(jg_index)

void jg_index_init(jg_index* index, size_t item_size);

void jg_index_cleanup(jg_index *index, void (*item_cleanup)(void *));

void* jg_index_add(jg_index* index, const char* key, const void* item);

void jg_index_build(jg_index* index);

void* jg_index_get(const jg_index* index, const char* key);

typedef struct jg_index_s {
    char** keys;
    void* items;
    size_t count;
    size_t size;
    size_t item_size;
    size_t packed_index;
} jg_index;

#endif

