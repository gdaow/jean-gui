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
#ifndef JGUI_PRIVATE_COMMON_DEBUG_H
#define JGUI_PRIVATE_COMMON_DEBUG_H

#include <stdbool.h>

#include "constants.h"

#if JG_DEBUG
void jg_assert_handler(
    bool test,
    const char* test_string,
    const char* file,
    int line
);
#   define JG_ASSERT(TEST)\
        (jg_assert_handler(TEST, #TEST, __FILE__, __LINE__))
#else
#   define JG_ASSERT(message, ...)
#endif

#endif
