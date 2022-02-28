/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * Associative container, using either linear search, or binary search.
 * Index can be packed in a user-provided buffer whose size can be returned by
 * jg_index_packed_size, so that it consumes only needed space and it's items
 * will be sorted, allowing to use binary search and thus make lookups quicker.
 *
 * This container is meant for one-time insert, then lookup-intensive usage.
 *
 */
#ifndef JGUI_PRIVATE_CONTAINERS_INDEX_H
#define JGUI_PRIVATE_CONTAINERS_INDEX_H

#include <stddef.h>

#include <jgui/misc/utils.h>

jg_forward_declare(jg_index)

/**
  * Initialize this index.
  *
  * @param index     Pointer on the index to initialize.
  * @param item_size Size of an item in the index. Generally use sizeof(stored_type).
  */
void jg_index_init(jg_index* index, size_t item_size);

/**
 * Cleans the index and the allocated memory.
 *
 * If the index was packed through jg_index_pack, the provided buffer will not be free.
 * Only memory allocated via jg_index_add will be free.
 *
 * @param item_cleanup Cleanup callback that will be called for every item in the index.
 */
void jg_index_cleanup(jg_index *index, void (*item_cleanup)(void *));

/**
 * Add an item to the index.
 *
 * The result is undefined if the given key is already in the index.
 *
 * @param key  The item key
 * @param item The item value, that will be copied in the index. You can pass NULL,
 *             and assign the item value through the returned pointer.
 *
 * @return Pointer to the item in the index. You can write this to set the item value
 *         through it instead of through the item parameter if more convenient.
 */
void* jg_index_add(jg_index* index, const char* key, const void* item);

/**
 * Get the size of the buffer that jg_index_pack needs to pack this index.
 *
 * @return The buffer size jg_index_pack needs to pack this index.
 */
size_t jg_index_packed_size(const jg_index* index);

/**
 * Pack this index in the given buffer, and sort it.
 *
 * Client is responsible of freeing the memory allocated in the given buffer.
 * Freeing the given memory can be done either after calling jg_vector_cleanup
 * on the given index, either after calling jg_index_pack with another buffer.
 *
 * @params buffer : Memory where to pack the index. The size of the given buffer
 *                  memory should be at least jg_index_packed_size(index) bytes.
 */
void jg_index_pack(jg_index* index, void* buffer);

/**
 * Get an item in the index given it's key.
 *
 * @params key : The key to search for.
 *
 * @return The item associated with the given key, or NULL if the given key is
 * not in the index.
 */
void* jg_index_get(const jg_index* index, const char* key);

/**
 * /!\ Private struct (see doc-private-struct) /!\
 */
typedef struct jg_index_s {
    void* _buffer;
    size_t _count;
    size_t _size;
    size_t _item_size;
    size_t _key_buffer_count;
    size_t _key_buffer_size;
    size_t _packed_index;
} jg_index;

#endif

