/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#if JG_DEBUG
void jg_assert_handler(
    bool test,
    const char* test_string,
    const char* file,
    int line
) {
    if(test) {
        return;
    }

    fprintf(stderr, "JEAN-GUI ASSERT FAILED:(%s:%i):%s", file, line, test_string);
    abort();
}

#endif
