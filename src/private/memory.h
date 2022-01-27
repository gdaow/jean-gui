/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * Custom allocators & memory helpers.
 *
 */
#ifndef JEAN_GUI_SRC_PRIVATE_MEMORY_H
#define JEAN_GUI_SRC_PRIVATE_MEMORY_H
#include <stddef.h>

static const size_t DEFAULT_CHUNK_SIZE = 0x1000;

// jg_arena allocator, used to store temporary data while creating modules or templates.
typedef struct jg_arena jg_arena;

// Uses DEFAULT_CHUNK_SIZE if chunk_size is 0
jg_arena* jg_arena_new(size_t chunk_size);
void* jg_arena_alloc(jg_arena* arena, size_t size, size_t align_size);
void jg_arena_free(jg_arena* arena);
char* jg_arena_strcpy(jg_arena* arena, const char* value, size_t max_length);

#endif
