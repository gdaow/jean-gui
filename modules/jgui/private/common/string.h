/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * TODO:
 *  - document.
 *
 */
#ifndef JGUI_PRIVATE_COMMON_STRING_H
#define JGUI_PRIVATE_COMMON_STRING_H

#include <stddef.h>

typedef struct jg_allocator_s jg_allocator;

size_t jg_strnlen(const char *s, size_t max_size);

char* jg_copy_string(jg_allocator* allocator, const char* value, size_t max_length);

char* jg_copy_identifier(jg_allocator* allocator, const char* value);

#endif
