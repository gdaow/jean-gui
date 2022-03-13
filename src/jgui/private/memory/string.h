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
#ifndef JGUI_PRIVATE_MEMORY_STRING_H
#define JGUI_PRIVATE_MEMORY_STRING_H

#include <stdbool.h>
#include <stddef.h>

size_t jg_strnlen(const char* s, size_t max_size);

bool jg_null_or_empty(const char* str);

#endif
