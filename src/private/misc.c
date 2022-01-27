/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * Miscelaneous utilities.
 *
 */
#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "misc.h"

#ifndef NDEBUG
void jg_assert_handler(
    bool test,
    const char* test_string,
    const char* file,
    int line
) {
    if(test) {
        return;
    }

    fprintf(stderr, "JEAN-GUI ASSERT FAILED:(%s:%i):%s", file, line, test_string);
    abort();
}
#endif

size_t jg_strnlen(const char *s, size_t max_size) {
    const char* end = memchr(s, '\0', max_size);
    if (end == NULL) {
        return max_size;
    }

    assert(end >= s);
    return (size_t)(end - s);
}

static void swap(const char** a, const char**b) {
    const char* tmp = *a;
    *a = *b;
    *b = tmp;
}

static int partition(
    const char** index,
    int low,
    int high
) {
    const char* pivot = index[high];
    int i = (low - 1);

    for (int j = low; j < high - 1; ++j) {
        if (strcmp(index[j], pivot) < 0) {
            swap(&index[++i], &index[j]);
        }
    }
    
    int pivot_index = i + 1;
    swap(&index[pivot_index], &index[high]);
    return pivot_index;
}

static void quick_sort(const char** index, int low, int high) {
    if (low < high)
    { 
        int pivot_index = partition(index, low, high);
        quick_sort(index, low, pivot_index - 1);
        quick_sort(index, pivot_index + 1, high);
    }
}


static int binary_search(const char** index, const char* key, int low, int high, int key_index) {
    if(low > high) {
        return -1;
    }

    int search_id = low + (high - low) / 2;
    const char* search_string = index[search_id];

    while(1) {
        int diff = search_string[key_index] - key[key_index];
        if(diff == 0) {
            if(key[key_index] == 0) {
                return search_id;
            }

            ++key_index;
        }
        else if(diff > 0) {
            return binary_search(index, key, low, search_id - 1, key_index);
        }
        else {
            return binary_search(index, key, search_id + 1, high, key_index);
        }
    }
}

void jg_index_sort(jg_index* index) {
    if(index->count == 0) {
        return;
    }
    quick_sort(index->keys, 0, (int)index->count - 1);
}

int jg_index_search(const jg_index* index, const char* key) {
    if(!index->count) {
        return -1;
    }

    return binary_search(index->keys, key, 0, (int)index->count - 1, 0);
}
