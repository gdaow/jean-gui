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
#ifndef JGUI_PUBLIC_JGUI_DEBUG_H
#define JGUI_PUBLIC_JGUI_DEBUG_H

#include <stdbool.h>

#ifdef NDEBUG
#   define JG_DEBUG 0
#else
#   define JG_DEBUG 1
#endif

#if JG_DEBUG
typedef void (*jg_assert_handler)(const char*, const char*, int);

void jg_set_assert_handler(jg_assert_handler handler);

void jg_assert(bool test, const char* expression, const char* file, int line);

#   define assert(TEST)\
        (jg_assert(TEST, #TEST, __FILE__, __LINE__))
#else
#   define assert(TEST)
#endif

#endif
