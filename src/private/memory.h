/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * Custom allocators.
 *
 */
#pragma once
#include <stddef.h>

/**
 * Helper to malloc and initialize a struct fields.
 * 
 * Usage :
 * User* user = ulg_malloc_struct(User,
 *     .name = "Mr Meeseeks",
 *     .age = 0.001
 * )
 */
#define ulg_malloc_struct(TYPE, ...)\
    memcpy(\
        malloc(sizeof(TYPE)),\
        &(TYPE) {\
            __VA_ARGS__\
        },\
        sizeof(TYPE)\
    )

static const size_t DEFAULT_CHUNK_SIZE = 0x1000;

typedef struct _UlgArena UlgArena;

void* _ulg_buffer_alloc(void* base, size_t* offset, size_t size, size_t align);

#define ulg_buffer_alloc(BASE, OFFSET, ...)\
    (memcpy(\
        _ulg_buffer_alloc(BASE, OFFSET, sizeof(*(__VA_ARGS__)), alignof(*(__VA_ARGS__))),\
        __VA_ARGS__,\
        sizeof(*(__VA_ARGS__))\
    ))

#define ulg_buffer_calloc(BASE, OFFSET, TYPE, SIZE)\
    (_ulg_buffer_alloc(BASE, OFFSET, SIZE*sizeof(TYPE), alignof(TYPE)))

/**
 * Create a new arena allocator.
 * 
 * @param chunk_size Size of allocated chunks.
 * @return The created arena.
 */
UlgArena* ulg_arena_new(size_t chunk_size);

/**
 * Allocate memory from an arena.
 * 
 * @param size size to allocate.
 * @param align_size Align the allocation to this size (use alignof operator)
 * @return The allocated memory.
 */
void* ulg_arena_alloc(UlgArena* arena, size_t size, size_t align_size);

/**
 * Release memory occupied by an arena.
 * 
 * @param arena 
 */
void ulg_arena_free(UlgArena* arena);

#define ulg_arena_alloc_struct(ARENA, ...)\
    (memcpy(\
        ulg_arena_alloc(ARENA, sizeof(*__VA_ARGS__), alignof(*__VA_ARGS__)),\
        __VA_ARGS__,\
        sizeof(*__VA_ARGS__)\
    ))

/**
 * @brief 
 * 
 * @param arena 
 * @param name 
 * @param max_length 
 * @return char* 
 */
char* ulg_arena_strcpy(UlgArena* arena, const char* name, size_t max_length);
