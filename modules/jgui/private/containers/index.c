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
#include <limits.h>

#include "jgui/private/memory/alloc.h"
#include "jgui/private/misc/assert.h"
#include "jgui/private/misc/utils.h"

/* Static methods */
typedef char* index_key;

static const size_t max_item_size = 1024;

static void quick_sort(jg_index* index, int low, int high);
static char* copy_key(const char* key);
static void* item_at(const jg_index* index, size_t id);

void jg_index_init(jg_index* index, size_t item_size) {
    assert(item_size < max_item_size);
    *index = (jg_index) {0};
    index->item_size = item_size;
}

void jg_index_cleanup(jg_index* index, void (*item_cleanup)(void*)) {
    size_t count = index->count;
    if(item_cleanup != NULL) {
        for(size_t i = 0; i < count; ++i) {
            void* item = item_at(index, i);
            item_cleanup(item);
        }
    }

    char** keys = index->keys;
    for(size_t key_id = 0; key_id < count; ++key_id) {
        jg_free(keys[key_id]);
    }
    jg_free(keys);
    jg_free(index->items);
}

void* jg_index_add(jg_index* index, const char* key, const void* item) {
    assert(index != NULL);
    assert(jg_index_get(index, key) == NULL);
    assert(index->count <= index->size);

    if(index->count == index->size) {
        index->size += (index->size + 1) * 2;
        index->items = jg_realloc(index->items, index->size * index->item_size);
        index->keys = jg_realloc(index->keys, index->size * sizeof(const char*));
    }

    size_t count = index->count;
    index->keys[count] = copy_key(key);

    void* dst_item = item_at(index, count);
    memcpy(dst_item, item, index->item_size);
    index->count += 1;
    return dst_item;
}

void jg_index_build(jg_index* index) {
    assert(index != NULL);
    size_t count = index->count;
    if(count == 0) {
        return;
    }
    assert(count < INT_MAX);
    quick_sort(index, 0, (int)count - 1);
    index->packed_index = count;
}

static int binary_search(
    char** index,
    const char* key,
    int low,
    int high
);

static void* item_at(const jg_index* index, size_t id) {
    char* item_data = index->items;
    return &(item_data[index->item_size * id]);
}

void* jg_index_get(const jg_index* index, const char* key) {
    assert(index != NULL);
    assert(key != NULL);

    size_t count = index->count;
    if(count == 0) {
        return NULL;
    }

    size_t packed_index = index->packed_index;
    size_t key_size = strlen(key);
    assert(key_size <= UCHAR_MAX);
    char** keys = index->keys;

    int result = binary_search(keys, key, 0, (int)packed_index - 1);
    if(result == -1) {
        for(size_t i = packed_index; i < index->count; ++i) {
            if(strcmp(keys[i], key) == 0) {
                return item_at(index, i);
            }
        }
    }

    if(result == -1) {
        return NULL;
    }

    assert(result >= 0 && (size_t)result < count);

    char* item_data = index->items;
    return &(item_data[(size_t)result * index->item_size]);
}

static int binary_search(char** index, const char* key, int low, int high) {
    assert(index != NULL);
    assert(key != NULL);

    if(low > high) {
        return -1;
    }

    int search_id = low + (high - low) / 2;
    const char* candidate = index[search_id];

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
    void* item_a = item_at(index, (size_t)a);
    void* item_b = item_at(index, (size_t)b);

    memcpy(tmp_item, item_a, item_size);
    memcpy(item_a, item_b, item_size);
    memcpy(item_b, tmp_item, item_size);

    char** keys = index->keys;
    char* tmp = keys[a];
    keys[a] = keys[b];
    keys[b] = tmp;
}

static int partition(jg_index* index, int low, int high) {
    assert(low < high);
    char** keys = index->keys;

    const char* pivot = keys[high];

    int i = (low - 1);\
    for (int j = low; j <= high - 1; ++j) {
        if (strcmp(keys[j], pivot) < 0) {
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

static char* copy_key(const char* key) {
    size_t length = strlen(key);
    assert(length < 255);
    char* string_copy = jg_malloc(sizeof(char) * (length + 1));
    strcpy(string_copy, key);
    return string_copy;
}

