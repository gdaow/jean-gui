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

typedef struct __ArenaChunk _ArenaChunk;

struct __ArenaChunk {
    _ArenaChunk* next;
    size_t offset;
};

struct __Arena {
    size_t chunk_size;
    _ArenaChunk* chunks;
};

_Arena* _arena_new(size_t chunk_size) {
    if(!chunk_size) {
        chunk_size = DEFAULT_CHUNK_SIZE;
    }

    return malloc_init(&(_Arena) {
        .chunk_size = chunk_size,
        .chunks = NULL
    });
}

static void* _chunk_alloc(_ArenaChunk* chunk, size_t chunk_size, size_t size, size_t align) {
    void* base = chunk + 1;
    size_t offset = chunk->offset;

    // align
    offset = offset + (align - offset % align);

    // reserve
    uintptr_t ptr = (uintptr_t)base + offset;
    offset = offset + size;

    // out of bounds : no enough availabe space
    if(offset > chunk_size)
        return NULL;

    chunk->offset = offset;
    return (void*)ptr;
}

static _ArenaChunk* _new_chunk(_Arena* arena) {
    _ArenaChunk* chunk = malloc(sizeof(_ArenaChunk) + arena->chunk_size);
    chunk->next = arena->chunks;
    chunk->offset = 0;
    arena->chunks = chunk;
    return chunk;
}

void* _arena_alloc(_Arena* arena, size_t size, size_t align) {
    _ArenaChunk* chunk = arena->chunks;
    size_t chunk_size = arena->chunk_size;
    void* result = NULL;

    // search chunks for free memory large enough
    while(chunk) {
        result = _chunk_alloc(chunk, chunk_size, size, align);

        if(result)
            return result;

        chunk = chunk->next;
    }

    // everyhing is full, we create a new chunk
    chunk = _new_chunk(arena);
    result = _chunk_alloc(chunk, chunk_size, size, align);
    assert(result); // alignment and size requirements don't fit in chunk_size

    return result;
}

void _arena_free(_Arena* arena) {
    _ArenaChunk* chunk = arena->chunks;
    while(chunk) {
        _ArenaChunk* next = chunk->next;
        free(chunk);
        chunk = next;
    }

    free(arena);
}

char* _arena_strcpy(_Arena* arena, const char* value, size_t max_length, size_t* length_accumulate) {
    size_t length = _ulg_strnlen(value, max_length) + 1;
    if(length_accumulate) {
        *length_accumulate += length;
    }
    char* copy = _arena_alloc(arena, length, alignof(char));
    memcpy(copy, value, length);
    copy[length] = '\0';
    return copy;
}
