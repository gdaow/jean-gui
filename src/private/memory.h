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
#ifndef UILIGI_SRC_PRIVATE_MEMORY_H
#define UILIGI_SRC_PRIVATE_MEMORY_H
#include <stddef.h>

static const size_t DEFAULT_CHUNK_SIZE = 0x1000;

// ulg_arena_t allocator, used to store temporary data while creating modules or templates.
typedef struct ulg_arena_t ulg_arena_t;

// Uses DEFAULT_CHUNK_SIZE if chunk_size is 0
ulg_arena_t* ulg_arena_new(size_t chunk_size);
void* ulg_arena_alloc(ulg_arena_t* arena, size_t size, size_t align_size);
void ulg_arena_free(ulg_arena_t* arena);

// length_accumulate is used to grow the total memory needed to store strings, in order to allocate
// the needed space when building modules. Length of the given string will be added to the pointed
// size_t.
char* ulg_arena_strcpy(ulg_arena_t* arena, const char* value, size_t max_length, size_t* length_accumulate);

#define ULG_MALLOC_INIT(...)\
    (memcpy(\
        malloc(sizeof(*(__VA_ARGS__))),\
        __VA_ARGS__,\
        sizeof(*(__VA_ARGS__))\
    ))

#define ULG_ARENA_ALLOC_INIT(ARENA, TYPE, ...)\
    (memcpy(\
        ulg_arena_alloc(ARENA, sizeof(TYPE), alignof(TYPE)),\
        __VA_ARGS__,\
        sizeof(TYPE)\
    ))

#endif
