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

#include <jgui/context.h>

#include "common/constants.h"
#include "common/debug.h"

struct jg_context_s {
    jg_error_handler error_handler;
};


static void default_error_handler(jg_error_code error_code, const char* message);

jg_context* jg_context_load(jg_plugin* plugins) {
    (void)plugins;
    return NULL;
}

jg_module_definition* jg_context_add_module(jg_context_definition* context_definition, const char* namespace) {
    (void)context_definition;
    (void)namespace;
    return NULL;
}

jg_context* jg_context_new() {
    jg_context* new_context = malloc(sizeof(jg_context));
    JG_ASSERT(new_context != NULL); // TODO(corentin@ki-dour.org) handle error.

    *new_context = (jg_context) {
        .error_handler = default_error_handler
    };

    return new_context;
}

void jg_context_free(jg_context* context) {
    JG_ASSERT(context != NULL);
    free(context);
}

jg_class* jg_context_get_class(jg_context* context, const char* namespace, const char* id) {
    (void)context;
    (void)namespace;
    (void)id;
    return NULL;
}

void jg_error(const jg_context* context, jg_error_code error_code, const char* format, ...) {
    JG_ASSERT(context != NULL);
    JG_ASSERT(context->error_handler != NULL);

    char error_message[JG_MAX_ERROR_MESSAGE_LENGTH];

    va_list arg_pointer;
    va_start(arg_pointer, format);
    vsnprintf(error_message, JG_MAX_ERROR_MESSAGE_LENGTH, format, arg_pointer);
    va_end(arg_pointer);

    context->error_handler(error_code, error_message);
}

void jg_set_error_handler(jg_context_definition* context_definition, jg_error_handler handler) {
    JG_ASSERT(context_definition != NULL);
    if(handler == NULL) {
        handler = default_error_handler;
    }
    //context->error_handler = handler;
}

static void default_error_handler(jg_error_code error_code, const char* message) {
    JG_ASSERT(message != NULL);
    printf("JG ERROR : %i : %s", error_code, message);
}
