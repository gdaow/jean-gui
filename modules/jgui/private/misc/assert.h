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
#ifndef JGUI_PRIVATE_MISC_ASSERT_H
#define JGUI_PRIVATE_MISC_ASSERT_H
#include <stdbool.h>

#include <jgui/static_config.h>

#if JG_ENABLE_ASSERTS
#   define assert(TEST) (jg_assert(TEST, #TEST, __FILE__, __LINE__))

    void jg_assert(bool test, const char* expression, const char* file, int line);

#else
#   define assert(TEST)
#endif

#endif
