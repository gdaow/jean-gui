/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#ifndef JGUI_PRIVATE_MEMORY_ALLOC_H
#define JGUI_PRIVATE_MEMORY_ALLOC_H

#include <stddef.h>

#include "jgui/static-config.h"

#if JG_MEMORY_DEBUG
void* jg_malloc_impl(size_t size, const char* file, int line);
void* jg_calloc_impl(size_t nb_elts,
                     size_t elt_size,
                     const char* file,
                     int line);
void* jg_realloc_impl(void* ptr, size_t size, const char* file, int line);
void jg_free_impl(void* ptr, const char* file, int line);

#	define jg_malloc(size) jg_malloc_impl(size, __FILE__, __LINE__)
#	define jg_calloc(nb_elt, elt_size) \
		jg_calloc_impl(nb_elt, elt_size, __FILE__, __LINE__)
#	define jg_realloc(ptr, size) jg_realloc_impl(ptr, size, __FILE__, __LINE__)
#	define jg_free(ptr) jg_free_impl(ptr, __FILE__, __LINE__)
#else
void* JG_WEAK jg_malloc_impl(size_t size);
void* JG_WEAK jg_calloc_impl(size_t nb_elts, size_t elt_size);
void* JG_WEAK jg_realloc_impl(void* ptr, size_t size);
void JG_WEAK jg_free_impl(void* ptr);

#	define jg_malloc(size) jg_alloc_impl(size)
#	define jg_calloc(nb_elts, elt_size) jg_calloc_impl(nb_elt, elt_size)
#	define jg_realloc(ptr, size) jg_realloc_impl(ptr, size)
#	define jg_free(ptr) jg_free_impl(ptr)
#endif

#endif
