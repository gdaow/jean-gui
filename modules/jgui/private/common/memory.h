/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#ifndef JGUI_PRIVATE_COMMON_ALLOCATOR_H
#define JGUI_PRIVATE_COMMON_ALLOCATOR_H

#include <stddef.h>

typedef void* (*jg_allocate_callback)(size_t, size_t, void*);
typedef void (*jg_free_callback)(void*, void*);

typedef struct jg_allocator_s {
    jg_allocate_callback allocate;
    jg_free_callback free;
    void* user_data;    
} jg_allocator;


/**
 * Allocate a chunk of memory using the given allocator.
 */
void* jg_allocate(jg_allocator* allocator, size_t count);

/**
 * Allocate a chuck of memory aligned to the given size.
 */
void* jg_allocate_aligned(jg_allocator* allocator, size_t count, size_t align);

/**
 * Free a chunk of memory using the given allocator.
 */
void jg_free(jg_allocator* allocator, void* pointer);

#endif
