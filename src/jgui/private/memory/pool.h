/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * TODO:
 *  - document.
 *
 */
#ifndef JGUI_PRIVATE_MEMORY_FIXED_POOL_H
#define JGUI_PRIVATE_MEMORY_FIXED_POOL_H

#include "jgui/misc/utils.h"
#include <stddef.h>

#include <jgui/core/config.h>
#include <jgui/static_config.h>

JG_IF_MEMORY_DEBUG(
    extern void* disabled_pool;
)

jg_forward_declare(jg_pool)

jg_pool* jg_pool_create(size_t size);

void* jg_pool_alloc(size_t size, size_t align);

void jg_pool_free(void* ptr);

void jg_pool_destroy(jg_pool* pool);

#define jg_pool_alloc(size, align)
#define jg_pool_calloc(nb_elements, element_size, align)
#define jg_pool_calloc_type(pool, nb_elements, element_type) (void)
#endif
