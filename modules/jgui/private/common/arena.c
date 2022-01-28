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

#include "arena.h"
#include "constants.h"
#include "debug.h"
#include "string.h"

typedef struct jg_arena_chunk_t jg_arena_chunk_t;

struct jg_arena_chunk_t {
    jg_arena_chunk_t* next;
    size_t offset;
};

struct jg_arena_s {
    size_t chunk_size;
    jg_arena_chunk_t* chunks;
};

jg_arena* jg_arena_new(size_t chunk_size) {
    if(chunk_size == 0) {
        chunk_size = JG_ARENA_DEFAULT_CHUNK_SIZE;
    }

    jg_arena* new_arena = malloc(sizeof(jg_arena));
    JG_ASSERT(new_arena != NULL); // TODO(corentin@ki-dour.org) handle error.

    *new_arena = (jg_arena) {
        .chunk_size = chunk_size,
        .chunks = NULL
    };

    return new_arena;
}

char* jg_copy_identifier(jg_arena* arena, const char* value) {
    JG_ASSERT(arena != NULL);
    JG_ASSERT(value != NULL);

    return jg_arena_strcpy(arena, value, JG_MAX_IDENTIFIER_LENGTH);
}

static jg_arena_chunk_t* new_chunk(jg_arena* arena);
static void* chunk_alloc(jg_arena_chunk_t* chunk, size_t chunk_size, size_t size, size_t align);

void* jg_arena_alloc(jg_arena* arena, size_t size, size_t align) {
    JG_ASSERT(arena != NULL);
    JG_ASSERT(size > 0);
    JG_ASSERT(align > 0);

    jg_arena_chunk_t* chunk = arena->chunks;
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
    JG_ASSERT(result != NULL); // alignment and size requirements don't fit in chunk_size

    return result;
}

void jg_arena_free(jg_arena* arena) {
    JG_ASSERT(arena != NULL);
    jg_arena_chunk_t* chunk = arena->chunks;
    while(chunk) {
        jg_arena_chunk_t* next = chunk->next;
        free(chunk);
        chunk = next;
    }

    free(arena);
}

char* jg_arena_strcpy(jg_arena* arena, const char* value, size_t max_length) {
    JG_ASSERT(arena != NULL);
    JG_ASSERT(value != NULL);

    size_t length = jg_strnlen(value, max_length) + 1;
    char* copy = jg_arena_alloc(arena, length, alignof(char));
    JG_ASSERT(copy != NULL); //TODO(corentin@ki-dour.org) handle error.
    memcpy(copy, value, length);
    copy[length] = '\0';
    return copy;
}

static jg_arena_chunk_t* new_chunk(jg_arena* arena) {
    JG_ASSERT(arena != NULL);

    jg_arena_chunk_t* chunk = malloc(sizeof(jg_arena_chunk_t) + arena->chunk_size);
    JG_ASSERT(chunk != NULL); // TODO(corentin@ki-dour.org) handle error.
    chunk->next = arena->chunks;
    chunk->offset = 0;
    arena->chunks = chunk;
    return chunk;
}


static void* chunk_alloc(jg_arena_chunk_t* chunk, size_t chunk_size, size_t size, size_t align) {
    JG_ASSERT(chunk != NULL);
    JG_ASSERT(chunk_size > 0);
    JG_ASSERT(align > 0);

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
