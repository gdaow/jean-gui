/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 */
#include "jgui/private/containers/vector.h"

#include <string.h>

#include "jgui/private/memory/alloc.h"
#include "jgui/private/misc/assert.h"

/* static utilities */
char* item_at(const jg_vector* vector, size_t item_index) {
    assert(item_index < vector->_size);
    return vector->_data + vector->_item_size * item_index;
}

jg_vector jg_vector_init(size_t item_size) {
    assert(item_size > 0);
    return (jg_vector) {
        0,
        ._item_size = item_size
    };
}

void jg_vector_cleanup(jg_vector* vector) {
    jg_free(vector->_data);
}

void jg_vector_push(jg_vector* vector, const void* item) {
    assert(vector != NULL);
    assert(item != NULL);

    size_t size  = vector->_size;
    size_t capacity = vector->_capacity;
    size_t item_size = vector->_item_size;
    assert(size <= capacity);

    if(capacity == size) {
        capacity = vector->_capacity = (capacity + 1) * 2;
        vector->_data = jg_realloc(vector->_data, capacity * item_size);
    }

    ++vector->_size;
    assert(vector->_size <= vector->_capacity);

    memcpy(item_at(vector, size), item, item_size);
}

void jg_vector_append(jg_vector* vector, const void* items, size_t item_count) {
    assert(vector != NULL);
    assert(items != NULL);
    // TODO(corentin@ki-dour.org) : Set capacity here.

    const char* items_data = items;
    for(size_t i = 0; i < item_count; ++i) {
        jg_vector_push(vector, items_data);
        items_data += vector->_item_size;
    }
}

const void* jg_vector_front(const jg_vector* vector) {
    return vector->_data;
}

const void* jg_vector_at(const jg_vector* vector, size_t index) {
    assert(vector->_size > 0);
    assert(vector->_size > index);
    return item_at(vector, index);
}

const void* jg_vector_back(const jg_vector* vector) {
    assert(vector->_size > 0);
    return item_at(vector, vector->_size - 1);
}

size_t jg_vector_size(const jg_vector* vector) {
    return vector->_size;
}

