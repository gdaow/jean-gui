/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include <stdalign.h>

#include "memory.h"
#include "debug.h"


void* jg_allocate(jg_allocator* allocator, size_t count) {
    JG_ASSERT(allocator != NULL);
    JG_ASSERT(count > 0);
    return allocator->allocate(count, alignof(max_align_t), allocator->user_data);
}

void* jg_allocate_aligned(jg_allocator* allocator, size_t count, size_t align) {
    JG_ASSERT(allocator != NULL);
    JG_ASSERT(count > 0);
    JG_ASSERT(align > 0);
    return allocator->allocate(count, align, allocator->user_data);
}

void jg_free(jg_allocator* allocator, void* pointer) {
    JG_ASSERT(allocator != NULL);
    JG_ASSERT(pointer != NULL);
    allocator->free(pointer, allocator->user_data);
}