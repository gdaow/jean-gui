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

#include <jgui/misc/utils.h>
#include <jgui/static-config.h>

#if JG_MEMORY_DEBUG

void* JG_WEAK jg_malloc_impl(size_t size, const char* file, int line) {
    (void)file;
    (void)line;
    return malloc(size);
}

void* JG_WEAK jg_calloc_impl(size_t nb_elts, size_t elt_size, const char* file, int line) {
    (void)file;
    (void)line;
    return calloc(nb_elts, elt_size);
}

void* JG_WEAK jg_realloc_impl(void* ptr, size_t size, const char* file, int line) {
    (void)file;
    (void)line;
    return realloc(ptr, size);
}

void JG_WEAK jg_free_impl(void* ptr, const char* file, int line) {
    (void)file;
    (void)line;
    free(ptr);
}

#else

void* JG_WEAK jg_malloc_impl(size_t size) {
    return malloc(size);
}

void* JG_WEAK jg_calloc_impl(size_t nb_elts, size_t elt_size) {
    return calloc(nb_elts, elt_size);
}

void* JG_WEAK jg_realloc_impl(void* ptr, size_t size) {
    return realloc(ptr, size);
}

void JG_WEAK jg_free_impl(void* ptr) {
    free(ptr);
}

#endif
