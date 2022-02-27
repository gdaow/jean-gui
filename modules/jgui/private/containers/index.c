/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 */
#include "jgui/private/containers/index.h"

#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "jgui/private/memory/alloc.h"
#include "jgui/private/misc/assert.h"
#include "jgui/private/misc/utils.h"

/* Static methods */
static const size_t max_item_size = 1024;
static const size_t average_key_size = 24;

static void* set_item_at(const jg_index* index, const void* value, size_t id);
static void* get_item_at(const jg_index* index, size_t id);
static void set_key_at(jg_index* index, const char* key, size_t id);
static char* get_key_at(const jg_index* index, size_t id);

static void index_realloc(jg_index* index, size_t new_size, size_t new_key_buffer_size);
static void index_grow(jg_index* index, size_t new_key_size);
static void quick_sort(jg_index* index, int low, int high);
static int binary_search(const jg_index* index, const char* key, int low, int high);

void jg_index_init(jg_index* index, size_t item_size) {
    assert(index != NULL);
    assert(item_size < max_item_size);

    *index = (jg_index) {0};
    index->item_size = item_size;
}

void jg_index_cleanup(jg_index* index, void (*item_cleanup)(void*)) {
    assert(index != NULL);

    if(item_cleanup != NULL) {
        for(size_t i = 0; i < index->count; ++i) {
            void* item = get_item_at(index, i);
            item_cleanup(item);
        }
    }

    jg_free(index->buffer);
}

void* jg_index_add(jg_index* index, const char* key, const void* item) {
    assert(index != NULL);
    assert(jg_index_get(index, key) == NULL);

    index_grow(index, strlen(key));

    size_t count = index->count++;

    set_key_at(index, key, count);
    return set_item_at(index, item, count);
}

void jg_index_pack(jg_index* index) {
    assert(index != NULL);

    size_t count = index->count;

    quick_sort(index, 0, (int)count - 1);
    index_realloc(index, count, index->key_buffer_count);
    index->sorted_index = count;
}

void* jg_index_get(const jg_index* index, const char* key) {
    assert(index != NULL);
    assert(key != NULL);

    size_t count = index->count;
    if(count == 0) {
        return NULL;
    }

    size_t sorted_index = index->sorted_index;

    int result = binary_search(index, key, 0, (int)sorted_index - 1);

    if(result == -1) {
        for(size_t i = sorted_index; i < index->count; ++i) {
            const char* candidate = get_key_at(index, i);
            if(strcmp(candidate, key) == 0) {
                return get_item_at(index, i);
            }
        }
    }

    if(result == -1) {
        return NULL;
    }

    assert(result >= 0 && (size_t)result < count);

    return get_item_at(index, (size_t)result);
}

static size_t* get_key_indices(const jg_index* index) {
    assert(index != NULL);

    return (size_t*)((char*)index->buffer + index->item_size * index->size);
}

static char* get_key_buffer(const jg_index* index) {
    assert(index != NULL);

    return ((char*)index->buffer + (index->item_size + sizeof(size_t)) * index->size);
}

static void* set_item_at(const jg_index* index, const void* value, size_t id) {
    void* result = get_item_at(index, id);

    if(value != NULL) {
        memcpy(result, value, index->item_size);
    }

    return result;
}

static void* get_item_at(const jg_index* index, size_t id) {
    assert(index != NULL);
    assert(id < index->count);

    char* item_data = index->buffer;
    return &(item_data[index->item_size * id]);
}

static void set_key_at(jg_index* index, const char* key, size_t id) {
    assert(index != NULL);
    assert(key != NULL);
    assert(id < index->count);

    size_t key_length = strlen(key) + 1; // for terminal null character
    char* key_buffer = get_key_buffer(index) + index->key_buffer_count;
    memcpy(key_buffer, key, key_length);

    size_t* key_indices = get_key_indices(index);
    key_indices[id] = index->key_buffer_count;

    index->key_buffer_count += key_length;
}

static char* get_key_at(const jg_index* index, size_t id) {
    assert(index != NULL);
    assert(index->count > id);

    size_t* key_indices = get_key_indices(index);
    char* key_buffer = get_key_buffer(index);
    size_t key_index = key_indices[id];
    assert(key_index < index->key_buffer_count);

    return &(key_buffer[key_index]);
}

static void index_realloc(jg_index* index, size_t new_size, size_t new_key_buffer_size) {
    assert(index != NULL);
    assert(new_size >= index->count);
    assert(new_key_buffer_size >= index->key_buffer_count);

    size_t count = index->count;
    size_t new_buffer_size = new_size * (index->item_size + sizeof(size_t)) + new_key_buffer_size;

    char* old_items = index->buffer;
    size_t* old_key_indices = get_key_indices(index);
    char* old_key_buffer = get_key_buffer(index);

    index->buffer = jg_malloc(new_buffer_size);
    index->size = new_size;
    index->key_buffer_size = new_key_buffer_size;

    memcpy(index->buffer, old_items, count * index->item_size);

    // copy keys one by one, as the order can have changed by sorting, so copy them in the index order.
    index->key_buffer_count = 0;
    for(size_t i = 0; i < count; ++i) {
        char* old_key = old_key_buffer + old_key_indices[i];
        set_key_at(index, old_key, i);
    }

    jg_free(old_items);
}

static void index_grow(jg_index* index, size_t new_key_size) {
    assert(index != NULL);

    size_t count = index->count;
    size_t size = index->size;
    size_t key_buffer_size = index->key_buffer_size;

    if(count == size) {
        size = (size + 1) * 2;
    }

    size_t key_buffer_count = index->key_buffer_count;
    if(key_buffer_count + new_key_size > key_buffer_size) {
        key_buffer_size = key_buffer_count + new_key_size + (size - count) * average_key_size;
    }

    if(size != index->size || key_buffer_size != index->key_buffer_size) {
        index_realloc(index, size, key_buffer_size);
    }

    assert(index->count <= index->size);
}

static int binary_search(const jg_index* index, const char* key, int low, int high) {
    assert(index != NULL);
    assert(key != NULL);

    if(low > high) {
        return -1;
    }

    int search_id = low + (high - low) / 2;
    const char* candidate = get_key_at(index, (size_t)search_id);
    int diff = strcmp(candidate, key);

    if(diff == 0) {
        return search_id;
    }

    if(diff > 0) {
        return binary_search(index, key, low, search_id - 1);
    }

    return binary_search(index, key, search_id + 1, high);
}

static void index_swap(jg_index* index, int a, int b) {
    assert(a >= 0 && (size_t)a < index->count);
    assert(b >= 0 && (size_t)b < index->count);

    size_t item_size = index->item_size;
    char tmp_item[max_item_size];
    void* item_a = get_item_at(index, (size_t)a);
    void* item_b = get_item_at(index, (size_t)b);

    memcpy(tmp_item, item_a, item_size);
    memcpy(item_a, item_b, item_size);
    memcpy(item_b, tmp_item, item_size);

    size_t* key_indices = get_key_indices(index);
    size_t tmp = key_indices[a];
    key_indices[a] = key_indices[b];
    key_indices[b] = tmp;
}

static int partition(jg_index* index, int low, int high) {
    assert(low < high);
    const char* pivot = get_key_at(index, (size_t)high);

    int i = (low - 1);\
    for (int j = low; j <= high - 1; ++j) {
        const char* key = get_key_at(index, (size_t)j);
        if (strcmp(key, pivot) < 0) {
            ++i;
            index_swap(index, i, j);
        }
    }
    int pivot_index = i + 1;
    index_swap(index, pivot_index, high);
    return pivot_index;
}

static void quick_sort(jg_index* index, int low, int high) {
    if (low < high) {
        int pivot_index = partition(index, low, high);
        quick_sort(index, low, pivot_index - 1);
        quick_sort(index, pivot_index + 1, high);
    }
}

