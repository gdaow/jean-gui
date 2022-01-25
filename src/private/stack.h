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

typedef struct ulg_stack_s ulg_stack_t;

struct ulg_stack_s {
    char* top;
    char* base;
    size_t size;
    size_t element_size;
};

void ulg_stack_init(ulg_stack_t* stack, size_t element_size);
void ulg_stack_clean(ulg_stack_t* stack);
void ulg_stack_push(ulg_stack_t* stack, void* element);
void* ulg_stack_pop(ulg_stack_t* stack);
