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

#include "jgui/private/misc/assert.h"
#include "jgui/private/misc/utils.h"

/* Static methods */
static void quick_sort(const char** keys, int low, int high);

void jg_index_init(jg_index* index, size_t item_size) {
    *index = (jg_index) {0};
    index->item_size = item_size;
}

void jg_index_cleanup(jg_index* index, void (*item_cleanup)(void*)) {
    (void)index;
    (void)item_cleanup;
}

void* jg_index_add(jg_index* index, const char* key) {
    (void)index;
    (void)key;
    return NULL;
}

void jg_index_build(jg_index* index) {
    (void)index;
}

void* jg_index_get(const jg_index* index, const char* key) {
    (void)index;
    (void)key;
    return NULL;
}

void jg_index_sort(const char** keys, size_t count) {
    assert(keys != NULL);
    if(count == 0) {
        return;
    }
    assert(count < INT_MAX);
    quick_sort(keys, 0, (int)count - 1);
}

static int binary_search(
    const char** index,
    const char* key,
    int low,
    int high,
    int current_char_index
);

int jg_index_search_key_(const char** index, const char* key, size_t count) {
    assert(index != NULL);
    assert(key != NULL);

    if(count == 0) {
        return -1;
    }

    int result = binary_search(index, key, 0, (int)count - 1, 0);
    assert(result == -1 || (result >= 0 && (size_t)result < count));
    return result;
}

static int binary_search(const char** index, const char* key, int low, int high, int current_char_index) {
    assert(index != NULL);
    assert(key != NULL);
    assert(current_char_index >= 0 && (size_t)current_char_index <= strlen(key));

    if(low > high) {
        return -1;
    }

    int search_id = low + (high - low) / 2;
    const char* candidate = index[search_id];
    //TODO(corentin@ki-dour.org) I think their's a bug here, we can search in the index keys past them ends.
    assert(current_char_index >= 0 && (size_t)current_char_index <= strlen(candidate));

    while(1) {
        int diff = candidate[current_char_index] - key[current_char_index];
        if(diff == 0) {
            if(key[current_char_index] == 0) {
                return search_id;
            }

            ++current_char_index;
        }
        else if(diff > 0) {
            return binary_search(index, key, low, search_id - 1, current_char_index);
        }
        else {
            return binary_search(index, key, search_id + 1, high, current_char_index);
        }
    }
}

static int partition(const char** keys, int low, int high) {\
    assert(low < high);

    const char* pivot = keys[high];

    int i = (low - 1);\
    for (int j = low; j <= high - 1; ++j) {
        if (strcmp(keys[j], pivot) < 0) {
            ++i;
            jg_swap(const char*, &keys[i], &keys[j]);
        }
    }
    int pivot_index = i + 1;
    jg_swap(const char*, &keys[pivot_index], &keys[high]);
    return pivot_index;
}

static void quick_sort(const char** keys, int low, int high) {
    if (low < high) {
        int pivot_index = partition(keys, low, high);
        quick_sort(keys, low, pivot_index - 1);
        quick_sort(keys, pivot_index + 1, high);
    }
}
