/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <jg/context.h>

#include "private/misc.h"


static const size_t JG_ERROR_MESSAGE_MAX_LENGTH = 1024;

struct jg_context_s {
    jg_error_handler error_handler;
};

static void default_error_handler(jg_error_code error_code, const char* message);

jg_context* jg_context_new() {
    jg_context* new_context = malloc(sizeof(jg_context));
    if(new_context == NULL) {
        return NULL;
    }

    *new_context = (jg_context) {
        .error_handler = default_error_handler
    };
    return new_context;
}

void jg_context_free(jg_context* context) {
    free(context);
}

void jg_error(const jg_context* context, jg_error_code error_code, const char* format, ...) {
    JG_ASSERT(context != NULL);
    JG_ASSERT(context->error_handler != NULL);

    char error_message[JG_ERROR_MESSAGE_MAX_LENGTH];

    va_list arg_pointer;
    va_start(arg_pointer, format);
    vsnprintf(error_message, JG_ERROR_MESSAGE_MAX_LENGTH, format, arg_pointer);
    va_end(arg_pointer);

    context->error_handler(error_code, error_message);
}

void jg_set_error_handler(jg_context* context, jg_error_handler handler) {
    JG_ASSERT(context != NULL);
    context->error_handler = handler;
}

static void default_error_handler(jg_error_code error_code, const char* message) {
    printf("JG ERROR : %i : %s", error_code, message);
}
