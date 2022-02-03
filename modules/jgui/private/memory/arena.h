/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 * TODO:
 *  - document.
 */
#ifndef JGUI_PRIVATE_MEMORY_ARENA_H
#define JGUI_PRIVATE_MEMORY_ARENA_H

#include <stddef.h>

#include <jgui/static_config.h>

typedef struct jg_arena_s jg_arena;

jg_arena* jg_arena_new(size_t default_chunk_size);
void jg_arena_destroy(jg_arena* arena);

char* jg_arena_copy_string(jg_arena* arena, const char* value, size_t max_length);
char* jg_arena_copy_identifier(jg_arena* arena, const char* value);

#if JG_MEMORY_DEBUG

#    define jg_arena_alloc(arena, size) jg_arena_alloc_(arena, size, __FILE__, __LINE__)
#    define jg_arena_free(ptr) jg_arena_free_(arena, ptr, __FILE__, __LINE__)

    void* jg_arena_alloc_(jg_arena* arena, size_t size, const char* file, int line);
    void jg_arena_free_(jg_arena* arena, void* ptr, const char* file, int line);

#else

#    define jg_arena_alloc(arena, size) jg_arena_alloc_(arena, size)
#    define jg_arena_free(ptr) // no-op : This is only usefull when debugging memory.

    void* jg_arena_alloc_(jg_arena* arena, size_t size);

#endif

#endif
