/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
 #include "jgui/private/containers/stack.h"

 #include <stdlib.h>
 #include <string.h>

 #include "jgui/private/misc/assert.h"
 #include "jgui/private/memory/alloc.h"

static const size_t GROW_SIZE = 0x10;

void jg_stack_init(jg_stack* stack, size_t element_size) {
    assert(stack != NULL);
    assert(element_size > 0);

    void* base = jg_calloc(GROW_SIZE, element_size);
    assert(stack != NULL); // TODO(corentin@ki-dour.org) handle error.
    *stack = (jg_stack) {
        .top = base,
        .base = base,
        .size = GROW_SIZE * element_size,
        .element_size = element_size
    };
}

void jg_stack_clean(jg_stack* stack) {
    assert(stack != NULL);
    jg_free(stack->base);
}

void jg_stack_push(jg_stack* stack, void* element) {
    assert(stack != NULL);
    assert(stack->base + stack->size >= stack->top);

    if(stack->base + stack->size == stack->top) { 
        size_t old_size = stack->size;
        stack->size = old_size + GROW_SIZE * stack->element_size;
        stack->base = jg_realloc(stack->base, stack->size);
        stack->top = stack->base + old_size;
    }

    memcpy(stack->top, element, stack->element_size);
    stack->top += stack->element_size;
}

void* jg_stack_peek(jg_stack* stack) {
    assert(stack != NULL);
    assert(stack->base + stack->size >= stack->top);
    return stack->top - stack->element_size;
}

void* jg_stack_pop(jg_stack* stack) {
    assert(stack != NULL);
    assert(stack->base <= stack->top - stack->element_size);

    stack->top -= stack->element_size;
    return stack->top;
}