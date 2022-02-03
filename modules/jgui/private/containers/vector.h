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
#ifndef JGUI_PRIVATE_CONTAINERS_VECTOR_H
#define JGUI_PRIVATE_CONTAINERS_VECTOR_H

#include <stddef.h>

typedef struct jg_vector_s {
    void* data;
    size_t item_size;
    size_t size;
    size_t count;
} jg_vector;

void jg_vector_new(jg_vector* vector, size_t item_size);

void jg_vector_push(jg_vector* vector, void* data);

void jg_vector_sort(jg_vector* vector, int (*compare_fuction)(void* a, void* b));

void jg_vector_search(jg_vector* vector, int)
#endif
