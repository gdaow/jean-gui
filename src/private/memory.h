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
#ifndef MEZO_SRC_PRIVATE_MEMORY_H
#define MEZO_SRC_PRIVATE_MEMORY_H
#include <stddef.h>

static const size_t DEFAULT_CHUNK_SIZE = 0x1000;

// mz_arena allocator, used to store temporary data while creating modules or templates.
typedef struct mz_arena mz_arena;

// Uses DEFAULT_CHUNK_SIZE if chunk_size is 0
mz_arena* mz_arena_new(size_t chunk_size);
void* mz_arena_alloc(mz_arena* arena, size_t size, size_t align_size);
void mz_arena_free(mz_arena* arena);

// length_accumulate is used to grow the total memory needed to store strings, in order to allocate
// the needed space when building modules. Length of the given string will be added to the pointed
// size_t.
char* mz_arena_strcpy(mz_arena* arena, const char* value, size_t max_length, size_t* length_accumulate);

#define MZ_MALLOC_INIT(...)\
    (memcpy(\
        malloc(sizeof(*(__VA_ARGS__))),\
        __VA_ARGS__,\
        sizeof(*(__VA_ARGS__))\
    ))

#define MZ_ARENA_ALLOC_INIT(ARENA, TYPE, ...)\
    (memcpy(\
        mz_arena_alloc(ARENA, sizeof(TYPE), alignof(TYPE)),\
        __VA_ARGS__,\
        sizeof(TYPE)\
    ))

#endif
