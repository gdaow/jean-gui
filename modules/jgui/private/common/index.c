/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 */
#include <string.h>

#include "debug.h"
#include "index.h"

static void index_sort(jg_index* index);

jg_index jg_index_build(
    const void* first_item,
    const char* (*get_id_and_next)(const void** next_item),
    void (*build_item)(const void* item, int sorted_id, void* user_data),
    const char*** index_pool,
    char** string_pool,
    void* user_data
 ) {
     JG_ASSERT(get_id_and_next != NULL);
     JG_ASSERT(build_item != NULL);
     JG_ASSERT(index_pool != NULL);

    const char** keys = *index_pool;
    const void* current_item = first_item;

    while(current_item) {
        // allocate a key in the index pool
        *((*index_pool)++) = get_id_and_next(&current_item);
    }

    JG_ASSERT(*index_pool >= keys);
    size_t count = (size_t)(*index_pool - keys); // total of allocated slots
 
    jg_index result = (jg_index) {
        .keys = keys,
        .count = count
    };

    index_sort(&result);

    // pack all sorted keys in the string pool : could improve cache efficieny when searching.
    for(size_t key_id = 0; key_id < count; ++key_id) {
        const char* key = keys[key_id];
        size_t key_length = strlen(key) + 1; // include terminal \0
        memcpy(*string_pool, keys[key_id], key_length);
        keys[key_id] = *string_pool;
        *string_pool += key_length;
    }
    
    current_item = first_item;
    while(current_item) {
        const void* next_item = current_item;
        const char* id = get_id_and_next(&next_item);
        int sorted_id = jg_index_search(&result, id);
        JG_ASSERT(sorted_id >= 0 && (size_t)sorted_id < count);
        build_item(current_item, sorted_id, user_data);
        current_item = next_item;
    }

    return result;
}

static int binary_search(const char** index, const char* key, int low, int high, int current_char_index);
int jg_index_search(const jg_index* index, const char* key) {
    JG_ASSERT(index != NULL);
    JG_ASSERT(key != NULL);

    if(!index->count) {
        return -1;
    }

    int result = binary_search(index->keys, key, 0, (int)index->count - 1, 0);
    JG_ASSERT(result == -1 || (result >= 0 && (size_t)result < index->count));
    return result;
}

static void swap(const char** a, const char**b) {
    JG_ASSERT(a != NULL);
    JG_ASSERT(b != NULL);
    const char* tmp = *a;
    *a = *b;
    *b = tmp;
}

static int partition(
    const char** index,
    int low,
    int high
) {
    JG_ASSERT(index != NULL);
    JG_ASSERT(low < high);

    const char* pivot = index[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; ++j) {
        if (strcmp(index[j], pivot) < 0) {
            swap(&index[++i], &index[j]);
        }
    }
    
    int pivot_index = i + 1;
    swap(&index[pivot_index], &index[high]);
    return pivot_index;
}

static void quick_sort(const char** index, int low, int high) {
    JG_ASSERT(index != NULL);
    if (low < high)
    { 
        int pivot_index = partition(index, low, high);
        quick_sort(index, low, pivot_index - 1);
        quick_sort(index, pivot_index + 1, high);
    }
}

static int binary_search(const char** index, const char* key, int low, int high, int current_char_index) {
    JG_ASSERT(index != NULL);
    JG_ASSERT(key != NULL);
    JG_ASSERT(current_char_index >= 0 && (size_t)current_char_index <= strlen(key));

    if(low > high) {
        return -1;
    }

    int search_id = low + (high - low) / 2;
    const char* candidate = index[search_id];
    //TODO(corentin@ki-dour.org) I think their's a bug here, we can search in the index keys past them ends.
    JG_ASSERT(current_char_index >= 0 && (size_t)current_char_index <= strlen(candidate));

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

static void index_sort(jg_index* index) {
    JG_ASSERT(index != NULL);

    if(index->count == 0) {
        return;
    }
    quick_sort(index->keys, 0, (int)index->count - 1);

    #if JG_DEBUG
        const char** keys = index->keys;
        for(size_t i = 0; i < index->count - 1; ++i) {
            JG_ASSERT(strcmp(keys[i], keys[i+1]) < 0);
        }
    #endif
}
