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
#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "misc.h"

typedef struct _UlgArenaChunk UlgArenaChunk;

struct _UlgArenaChunk {
    UlgArenaChunk* next;
    size_t offset;
};

struct _UlgArena {
    size_t chunk_size;
    UlgArenaChunk* chunks;
};

void* _ulg_buffer_alloc(void* base, size_t* offset, size_t size, size_t align) {
    *offset = *offset + (align - *offset % align);
    uintptr_t ptr = (uintptr_t)base + *offset;
    *offset = *offset + size;
    return (void*)ptr;
}

UlgArena* ulg_arena_new(size_t chunk_size) {
    UlgArena* result = ulg_malloc_struct(UlgArena, 
        .chunk_size = chunk_size,
        .chunks = NULL,
    );
    return result;
}

static UlgArenaChunk* _new_chunk(UlgArena* arena) {
    UlgArenaChunk* chunk = malloc(sizeof(UlgArenaChunk) + arena->chunk_size);
    chunk->next = arena->chunks;
    chunk->offset = 0;
    arena->chunks = chunk;
    return chunk;
}

void* ulg_arena_alloc(UlgArena* arena, size_t size, size_t align) {
    UlgArenaChunk* chunk = arena->chunks;
    size_t chunk_size = arena->chunk_size;
    void* result = NULL;

    // Search chunks free memory large enough
    while(chunk) {
        size_t offset = chunk->offset;
        result = _ulg_buffer_alloc(chunk + 1, &offset, size, align);

        if(offset <= chunk_size)
        {
            chunk->offset = offset;
            return result;
        }

        chunk = chunk->next;
    }

    // Everyhing is full, we create a new chunk
    chunk = _new_chunk(arena);
    result = _ulg_buffer_alloc(chunk + 1, &chunk->offset, size, align);
    assert(chunk->offset < chunk_size); // Alignment and size requirements don't fit in chunk_size

    return result;
}

void ulg_arena_free(UlgArena* arena) {
    UlgArenaChunk* chunk = arena->chunks;
    while(chunk) {
        UlgArenaChunk* next = chunk->next;
        free(chunk);
        chunk = next;
    }

    free(arena);
}

char* ulg_arena_strcpy(UlgArena* arena, const char* name, size_t max_length) {
    size_t name_len = ulg_strnlen(name, max_length);
    char* name_copy = ulg_arena_alloc(arena, name_len, alignof(char));
    memcpy(name_copy, name, name_len);
    name_copy[name_len] = '\0';
    return name_copy;
}
