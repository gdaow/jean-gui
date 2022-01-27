/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <assert.h>
#include <stdlib.h>

#include "jg/context.h"


jg_context* jg_context_new() {
    return NULL;
}

void jg_context_free(jg_context* context) {
    (void)context;
}

void jg_error(const jg_context* context, jg_error_code error_code, const char* format, ...) {
    (void)context;
    (void)error_code;
    (void)format;
}

void jg_set_error_handler(jg_context* context, jg_error_handler handler) {
    (void)context;
    (void)handler;
}
