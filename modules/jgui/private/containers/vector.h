/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * Generic vector implementation, encapsulating a dynamically growing
 * and shrinking array.
 *
 */
#ifndef JGUI_PRIVATE_CONTAINERS_VECTOR_H
#define JGUI_PRIVATE_CONTAINERS_VECTOR_H

#include <stddef.h>

#include <jgui/misc/utils.h>

jg_forward_declare(jg_vector)

/**
  * Initialize this vector.
  *
  * @param vector    Pointer on the vector to initialize.
  * @param item_size Size of an item in the array. Generally use sizeof(stored_type).
  */
jg_vector jg_vector_init(size_t item_size);

/**
 * Helper macro to initialize a vector with item_size set as the size of the given type.
 *
 * @param type Type of items stored in the vector.
 *
 * @return The initialized vector.
 */
#define jg_vector_of(type) jg_vector_init(sizeof(type))

/**
  * Clean memory used by this vector.
  *
  * @return The initialized vector.
  */
void jg_vector_cleanup(jg_vector* vector);

/**
  * Adds an item at the end of this vector.
  *
  * @param item Pointer to the item to add. The item will be copied in the vector.
  *             If the memory pointed by item is smaller than the vector's item size,
  *             the behavior is undefined.
  */
void jg_vector_push(jg_vector* vector, const void* item);

/**
  * Adds an array of items at the end of this vector.
  *
  * @param items Pointer to an array of items.
  */
void jg_vector_append(jg_vector* vector, const void* items, size_t item_count);

/**
  * Get the first item of this vector.
  *
  * @return Pointer to the first item of this vector.
  */
const void* jg_vector_front(const jg_vector* vector);

/**
  * Get the item at the given index in this vector.
  * If index is not in the bounds of the underlying array, the behavior is undefined.
  *
  * @param index Get the item at this index.
  *
  * @return Pointer to the item at the given index in this vector.
  */
const void* jg_vector_at(const jg_vector* vector, size_t index);

/**
  * Get the last item of this vector.
  *
  * @return Pointer to the last item of this vector.
  */
const void* jg_vector_back(const jg_vector* vector);

/**
  * Get the current number of items stored in this vector.
  *
  * @return The number of items.
  */
size_t jg_vector_size(const jg_vector* vector);

/**
 * /!\ Private struct (see doc-private-struct) /!\
 */
typedef struct jg_vector_s {
    char* _data;
    const size_t _item_size;
    size_t _size;
    size_t _capacity;
} jg_vector;

#endif

