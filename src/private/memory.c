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
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "misc.h"

typedef struct ulg_arena_chunk_t ulg_arena_chunk_t;

struct ulg_arena_chunk_t {
    ulg_arena_chunk_t* next;
    size_t offset;
};

struct ulg_arena_t {
    size_t chunk_size;
    ulg_arena_chunk_t* chunks;
};

ulg_arena_t* ulg_arena_new(size_t chunk_size) {
    if(!chunk_size) {
        chunk_size = DEFAULT_CHUNK_SIZE;
    }

    return ULG_MALLOC_INIT(&(ulg_arena_t) {
        .chunk_size = chunk_size,
        .chunks = NULL
    });
}

static void* chunk_alloc(ulg_arena_chunk_t* chunk, size_t chunk_size, size_t size, size_t align) {
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

static ulg_arena_chunk_t* new_chunk(ulg_arena_t* arena) {
    ulg_arena_chunk_t* chunk = malloc(sizeof(ulg_arena_chunk_t) + arena->chunk_size);
    chunk->next = arena->chunks;
    chunk->offset = 0;
    arena->chunks = chunk;
    return chunk;
}

void* ulg_arena_alloc(ulg_arena_t* arena, size_t size, size_t align) {
    ulg_arena_chunk_t* chunk = arena->chunks;
    size_t chunk_size = arena->chunk_size;
    void* result = NULL;

    // search chunks for free memory large enough
    while(chunk) {
        result = chunk_alloc(chunk, chunk_size, size, align);

        if(result) {
            return result;
}

        chunk = chunk->next;
    }

    // everyhing is full, we create a new chunk
    chunk = new_chunk(arena);
    result = chunk_alloc(chunk, chunk_size, size, align);
    assert(result); // alignment and size requirements don't fit in chunk_size

    return result;
}

void ulg_arena_free(ulg_arena_t* arena) {
    ulg_arena_chunk_t* chunk = arena->chunks;
    while(chunk) {
        ulg_arena_chunk_t* next = chunk->next;
        free(chunk);
        chunk = next;
    }

    free(arena);
}

char* ulg_arena_strcpy(ulg_arena_t* arena, const char* value, size_t max_length, size_t* length_accumulate) {
    size_t length = ulg_strnlen(value, max_length) + 1;
    if(length_accumulate) {
        *length_accumulate += length;
    }
    char* copy = ulg_arena_alloc(arena, length, alignof(char));
    memcpy(copy, value, length);
    copy[length] = '\0';
    return copy;
}
