/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include "jgui/private/memory/arena.h"

#include <stdalign.h>
#include <string.h>

#include <jgui/static_config.h>

#include "jgui/private/memory/alloc.h"
#include "jgui/private/memory/string.h"
#include "jgui/private/misc/assert.h"

typedef struct jg_arena_chunk_s {
    struct jg_arena_chunk_s* next_chunk;
    size_t size;
    size_t offset;
} jg_arena_chunk;

typedef struct jg_arena_s {
    jg_arena_chunk* first_chunk;
    size_t default_chunk_size;
} jg_arena;

static void* arena_alloc(jg_arena* arena, size_t size, size_t align);
static jg_arena_chunk* chunk_new(jg_arena_chunk* next_chunk, size_t size);
static void* chunk_alloc(jg_arena_chunk* chunk, size_t size, size_t align);

JG_IF_MEMORY_DEBUG(
    static jg_arena disabled_arena;
)

jg_arena* jg_arena_new(size_t default_chunk_size) {
    assert(default_chunk_size > sizeof(jg_arena) + sizeof(jg_arena_chunk));

    /*
    JG_IF_MEMORY_DEBUG(
        if(config & JG_CONFIG_DISABLE_ALLOCATORS) {
            return &disabled_arena;
        }
    )
    */

    if(default_chunk_size == 0) {
        default_chunk_size = JG_ARENA_DEFAULT_CHUNK_SIZE;
    }

    jg_arena_chunk* first_chunk = chunk_new(NULL, default_chunk_size);
    if(first_chunk == NULL) {
        return NULL;
    }

    // we store the jg_arena object in itself, it saves an allocation.
    jg_arena* new_arena = chunk_alloc(first_chunk, sizeof(jg_arena), alignof(jg_arena));
    assert(new_arena != NULL);

    *new_arena = (jg_arena) {
        .default_chunk_size = default_chunk_size,
        .first_chunk = first_chunk
    };

    return new_arena;
}

void jg_arena_destroy(jg_arena* arena) {
    JG_IF_MEMORY_DEBUG(
        if(arena == &disabled_arena) {
            return; // Everything should have been freed by jg_arena_free
        }
    )

    jg_arena_chunk* chunk = arena->first_chunk;

    while(chunk) {
        jg_arena_chunk* next = chunk->next_chunk;
        jg_free(chunk);
        chunk = next;
    }
}

char* jg_arena_copy_string(jg_arena* arena, const char* value, size_t max_length) {
    assert(arena != NULL);
    assert(value != NULL);

    size_t length = jg_strnlen(value, max_length) + 1;
    char* copy = jg_arena_alloc(arena, length);
    if(copy == NULL) {
        return NULL;
    }

    memcpy(copy, value, length);
    copy[length] = '\0';
    return copy;
}


char* jg_arena_copy_identifier(jg_arena* arena, const char* value) {
    return jg_arena_copy_string(arena, value, JG_MAX_IDENTIFIER_LENGTH);
}

#if JG_MEMORY_DEBUG
void* jg_arena_alloc_(jg_arena* arena, size_t size, const char* file, int line) {
    if(arena == &disabled_arena) {
        return jg_malloc_impl(size, file, line);
    }

    return arena_alloc(arena, size, alignof(max_align_t));
}

void jg_arena_free_(jg_arena* arena, void* ptr, const char* file, int line) {
    if(arena == &disabled_arena) {
        jg_free_impl(ptr, file, line);
    }

    // no-op : free will occur when destroying the whole arena
}
#else
void* jg_arena_alloc_(jg_arena* arena, size_t size) {
    return arena_alloc(arena, size, alignof(max_align_t));
}

void jg_arena_free_(jg_arena* arena, void* ptr) {
    // no-op : free will occur when destroying the whole arena
}
#endif

static jg_arena_chunk* chunk_new(jg_arena_chunk* next_chunk, size_t size) {
    jg_arena_chunk* new_chunk = jg_malloc(sizeof(jg_arena_chunk) + size);

    if(new_chunk == NULL) {
        return NULL;
    }

    *new_chunk = (jg_arena_chunk) {
        .size = size,
        .offset = 0,
        .next_chunk = next_chunk
    };

    return new_chunk;
}

static void* chunk_alloc(jg_arena_chunk* chunk, size_t size, size_t align) {
    assert(chunk != NULL);
    assert(align > 0);

    void* base = chunk + 1;
    size_t offset = chunk->offset;

    // align
    offset = offset + (align - offset % align);

    // reserve
    void* ptr = (char*)base + offset;
    offset = offset + size;

    // out of bounds : no enough availabe space
    if(offset > chunk->size) {
        return NULL;
    }

    chunk->offset = offset;
    return ptr;
}

static void* arena_alloc(jg_arena* arena, size_t size, size_t align) {
    assert(size > 0);
    assert(align > 0);

    jg_arena_chunk* chunk = arena->first_chunk;
    void* result = NULL;

    // search chunks for free memory large enough
    while(chunk) {
        result = chunk_alloc(chunk, size, align);

        if(result) {
            return result;
        }

        chunk = chunk->next_chunk;
    }

    // everyhing is full, we create a new chunk
    size_t default_size = arena->default_chunk_size;
    size_t chunk_size = size > default_size ? size : default_size;
    jg_arena_chunk* new_chunk = chunk_new(arena->first_chunk, chunk_size);

    if(new_chunk == NULL) {
        return NULL;
    }

    arena->first_chunk = new_chunk;

    result = chunk_alloc(arena->first_chunk, size, align);
    assert(result != NULL); //TODO(corentin@ki-dour.org) handle error.

    return result;
}
