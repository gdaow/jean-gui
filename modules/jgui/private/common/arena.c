/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include <stdalign.h>
#include <stdlib.h>
#include <string.h>

#include <jgui/debug.h>

#include "arena.h"
#include "constants.h"
#include "memory.h"
#include "string.h"

typedef struct jg_arena_chunk_s jg_arena_chunk;

struct jg_arena_chunk_s {
    jg_arena_chunk* next_chunk;
    size_t offset;
};

typedef struct jg_arena_s {
    jg_allocator allocator;
    size_t chunk_size;
    jg_arena_chunk* first_chunk;
} jg_arena;

static void* arena_alloc(size_t size, size_t align, void* user_data);
static void arena_free(void* pointer, void* user_data);

jg_allocator* jg_arena_new(size_t chunk_size) {
    if(chunk_size == 0) {
        chunk_size = JG_ARENA_DEFAULT_CHUNK_SIZE;
    }

    jg_arena* new_arena = malloc(sizeof(jg_arena));
    assert(new_arena != NULL); // TODO(corentin@ki-dour.org) handle error.

    *new_arena = (jg_arena) {
        .allocator = (jg_allocator) {
            .allocate = arena_alloc,
            .free = arena_free,
            .user_data = new_arena,
        },
        .chunk_size = chunk_size,
        .first_chunk = NULL
    };

    return &new_arena->allocator;
}

void jg_arena_free(jg_allocator* allocator) {
    assert(allocator != NULL);
    jg_arena* arena = allocator->user_data;
    jg_arena_chunk* chunk = arena->first_chunk;
    while(chunk) {
        jg_arena_chunk* next = chunk->next_chunk;
        free(chunk);
        chunk = next;
    }

    free(arena);
}

static jg_arena_chunk* new_chunk(jg_arena* arena) {
    assert(arena != NULL);

    jg_arena_chunk* chunk = malloc(sizeof(jg_arena_chunk) + arena->chunk_size);
    assert(chunk != NULL); // TODO(corentin@ki-dour.org) handle error.
    chunk->next_chunk = arena->first_chunk;
    chunk->offset = 0;
    arena->first_chunk = chunk;
    return chunk;
}

static void* chunk_alloc(jg_arena_chunk* chunk, size_t chunk_size, size_t size, size_t align) {
    assert(chunk != NULL);
    assert(chunk_size > 0);
    assert(align > 0);

    void* base = chunk + 1;
    size_t offset = chunk->offset;

    // align
    offset = offset + (align - offset % align);

    // reserve
    void* ptr = (char*)base + offset;
    offset = offset + size;

    // out of bounds : no enough availabe space
    if(offset > chunk_size) {
        return NULL;
    }

    chunk->offset = offset;
    return ptr;
}

static void* arena_alloc(size_t size, size_t align, void* user_data) {
    assert(size > 0);
    assert(align > 0);
    assert(user_data != NULL);
    jg_arena* arena = user_data;

    jg_arena_chunk* chunk = arena->first_chunk;
    size_t chunk_size = arena->chunk_size;
    void* result = NULL;

    // search chunks for free memory large enough
    while(chunk) {
        result = chunk_alloc(chunk, chunk_size, size, align);

        if(result) {
            return result;
        }

        chunk = chunk->next_chunk;
    }

    // everyhing is full, we create a new chunk
    chunk = new_chunk(arena);
    result = chunk_alloc(chunk, chunk_size, size, align);
    assert(result != NULL); // alignment and size requirements don't fit in chunk_size

    return result;
}

static void arena_free(void* pointer, void* user_data) {
    (void)pointer;
    (void)user_data;
}
