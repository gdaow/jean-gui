/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "jgui/private/core/context.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jgui/core/error_code.h>
#include <jgui/core/id.h>

#include "jgui/private/core/core_plugin.h"
#include "jgui/private/memory/alloc.h"
#include "jgui/private/containers/index.h"
#include "jgui/private/object-model/module.h"
#include "jgui/private/misc/assert.h"

/* structures definitions */

struct jg_context_s {
    jg_index module_index;
    jg_error_handler error_handler;
};

/* static definitions */

/* public API */

jg_context* jg_context_load(jg_plugin* plugins) {
    jg_context* context = jg_malloc(sizeof(jg_context));
    *context = (jg_context) {
        .error_handler = NULL,
    };

    jgui_core_plugin(context);

    if(plugins) {
        jg_plugin* current_plugin = plugins;
        while(*current_plugin) {
            (**current_plugin)(context);
            ++current_plugin;
        }
    }

    return context; // TODO(corentin@ki-dour.org) : build indexes.
}

void jg_context_free(jg_context* context) {
    assert(context != NULL);
    jg_free(context);
}

jg_class* jg_context_add_class(jg_context* context, jg_id id, jg_id parent_id, size_t size) {
    assert(context != NULL);
    assert(jg_valid_id(id));
    assert(jg_valid_id(parent_id));

    const jg_class* parent = NULL;
    if(!jg_empty_id(parent_id)) {
        parent = jg_context_get_class(context, parent_id);
    }

    jg_index* module_index = &context->module_index;

    jg_module* module = jg_index_get(module_index, id.namespace);
    if(module == NULL) {
        module = jg_index_add(module_index, id.namespace);
        jg_module_init(module);
    }

    return jg_module_add_class(module, id.name, parent, size);
}

const jg_class* jg_context_get_class(const jg_context* context, jg_id id) {
    assert(context != NULL);
    assert(jg_valid_id(id));

    const jg_module* module = jg_index_get(&context->module_index, id.namespace);
    if(module == NULL) {
        return NULL;
    }

    return jg_module_get_class(module, id.name);
}

void jg_error(const jg_context* context, jg_error_code error_code, const char* format, ...) {
    assert(context != NULL);
    assert(context->error_handler != NULL);

    char error_message[JG_MAX_ERROR_MESSAGE_LENGTH];

    va_list arg_pointer;
    va_start(arg_pointer, format);
    vsnprintf(error_message, JG_MAX_ERROR_MESSAGE_LENGTH, format, arg_pointer);
    va_end(arg_pointer);

    context->error_handler(error_code, error_message);
}

static void default_error_handler(jg_error_code error_code, const char* message) {
    assert(message != NULL);
    printf("JG ERROR : %i : %s", error_code, message);
}

void jg_set_error_handler(jg_context* context, jg_error_handler handler) {
    assert(context != NULL);
    if(handler == NULL) {
        handler = default_error_handler;
    }
    context->error_handler = handler;
}
