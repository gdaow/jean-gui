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
#pragma once
#include <stddef.h>

static const size_t DEFAULT_CHUNK_SIZE = 0x1000;

// _Arena allocator, used to store temporary data while creating modules or templates.
typedef struct __Arena _Arena;

// Uses DEFAULT_CHUNK_SIZE if chunk_size is 0
_Arena* _arena_new(size_t chunk_size);
void* _arena_alloc(_Arena* arena, size_t size, size_t align_size);
void _arena_free(_Arena* arena);

// length_accumulate is used to grow the total memory needed to store strings, in order to allocate
// the needed space when building modules. Length of the given string will be added to the pointed
// size_t.
char* _arena_strcpy(_Arena* arena, const char* value, size_t max_length, size_t* length_accumulate);

#define malloc_init(...)\
    (memcpy(\
        malloc(sizeof(*(__VA_ARGS__))),\
        __VA_ARGS__,\
        sizeof(*(__VA_ARGS__))\
    ))

#define _arena_alloc_init(ARENA, ...)\
    (memcpy(\
        _arena_alloc(ARENA, sizeof(*(__VA_ARGS__)), alignof(*(__VA_ARGS__))),\
        __VA_ARGS__,\
        sizeof(*(__VA_ARGS__))\
    ))
