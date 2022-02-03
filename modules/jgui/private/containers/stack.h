/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * TODO:
 *  - document.
 *  - Return the pointer of the pushed element with push, to avoid using
 *    memcpy to copy the value, and use direct assignment.
 */
 #ifndef JGUI_PRIVATE_CONTAINERS_STACK_H
 #define JGUI_PRIVATE_CONTAINERS_STACK_H

 #include <stddef.h>

typedef struct jg_stack_s {
    char* top;
    char* base;
    size_t size;
    size_t element_size;
} jg_stack;

void jg_stack_init(jg_stack* stack, size_t element_size);
void jg_stack_clean(jg_stack* stack);
void jg_stack_push(jg_stack* stack, void* element);
void* jg_stack_peek(jg_stack* stack);
void* jg_stack_pop(jg_stack* stack);

#endif
