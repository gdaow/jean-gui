/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include "jgui/private/memory/alloc.h"

#include <stdlib.h>

#include <jgui/static_config.h>

#if JG_MEMORY_DEBUG

void* jg_malloc_(size_t size, const char* file, int line) {
    (void)file;
    (void)line;
    return malloc(size);
}

void* jg_calloc_(size_t nb_elts, size_t elt_size, const char* file, int line) {
    (void)file;
    (void)line;
    return calloc(nb_elts, elt_size);
}

void* jg_realloc_(void* ptr, size_t size, const char* file, int line) {
    (void)file;
    (void)line;
    return realloc(ptr, size);
}

void jg_free_(void* ptr, const char* file, int line) {
    (void)file;
    (void)line;
    free(ptr);
}

#else

void* jg_malloc_(size_t size) {
    return malloc(size);
}

void* jg_calloc_(size_t nb_elts, size_t elt_size) {
    return calloc(nb_elts, elt_size);
}

void* jg_realloc_(void* ptr, size_t size) {
    return realloc(ptr, size);
}

void jg_free_(void* ptr) {
    free(ptr);
}

#endif
