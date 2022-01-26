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
 #include <stddef.h>

typedef struct mz_stack_s mz_stack;

struct mz_stack_s {
    char* top;
    char* base;
    size_t size;
    size_t element_size;
};

void mz_stack_init(mz_stack* stack, size_t element_size);
void mz_stack_clean(mz_stack* stack);
void mz_stack_push(mz_stack* stack, void* element);
void* mz_stack_peek(mz_stack* stack);
void* mz_stack_pop(mz_stack* stack);
