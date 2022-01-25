/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * Simple stack implementation.
 *
 */
 #include <assert.h>
 #include <stddef.h>
 #include <stdint.h>
 #include <stdlib.h>
 #include <string.h>

 #include "stack.h"

static const size_t GROW_SIZE = 0x10;

void ulg_stack_init(ulg_stack_t* stack, size_t element_size) {
    void* base = calloc(GROW_SIZE, element_size);
    *stack = (ulg_stack_t) {
        .top = base,
        .base = base,
        .size = GROW_SIZE,
        .element_size = element_size
    };
}

void ulg_stack_clean(ulg_stack_t* stack) {
    free(stack->base);
}

void ulg_stack_push(ulg_stack_t* stack, void* element) {
    assert(stack->base + stack->size <= stack->top);
    if(stack->base + stack->size == stack->top) { 
        size_t old_size = stack->size;
        stack->size = old_size + GROW_SIZE * stack->element_size;
        stack->base = realloc(stack->base, stack->size);
        stack->top = stack->base + old_size;
    }

    memcpy(stack->top, element, stack->element_size);
    stack->top += stack->element_size;
}

void* ulg_stack_pop(ulg_stack_t* stack) {
    void* result = NULL;
    if(stack->base != stack->top) {
        result = stack->top -= stack->element_size;
    }
    assert(stack->base <= stack->top);
    return result;
}
