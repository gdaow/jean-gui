/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <stdalign.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jgui/context.h>
#include "context.h"

#include "common/arena.h"
#include "common/constants.h"
#include "common/debug.h"
#include "common/memory.h"
#include "common/string.h"
#include "jgui/module.h"
#include "module.h"
#include "core_plugin.h"
#include "class.h"

struct jg_context_s {
    jg_error_handler error_handler;
    jg_pool pool;
    jg_index module_index;
};

typedef struct jg_context_builder_s {
    jg_allocator* allocator;
    jg_module_builder* first_module;
    jg_error_handler error_handler;
} jg_context_builder;

static void default_error_handler(jg_error_code error_code, const char* message);

static jg_context* build_context(jg_context_builder* context_builder);

jg_context* jg_context_load(jg_plugin* plugins) {
    jg_allocator* allocator = jg_arena_new(0);
    jg_context_builder* context_builder = jg_allocate_aligned(
        allocator,
        sizeof(jg_context_builder),
        alignof(jg_context_builder)
    );

    JG_ASSERT(context_builder != NULL); // TODO(corentin@ki-dour.org) handle error.

    *context_builder = (jg_context_builder) {
        .allocator = allocator,
        .first_module = NULL
    };

    jgui_core_plugin(context_builder);
    jg_plugin* current_plugin = plugins;
    while(*current_plugin) {
        (**current_plugin)(context_builder);
        ++current_plugin;
    }

    return build_context(context_builder);
}

jg_module_builder* jg_context_add_module(jg_context_builder* context_builder, const char* namespace) {
    (void)context_builder;
    (void)namespace;
    jg_allocator* allocator = jg_arena_new(0);
    jg_module_builder* module_builder = jg_allocate_aligned(
        allocator,
        sizeof(jg_module_builder),
        alignof(jg_module_builder)
    );

    JG_ASSERT(module_builder != NULL); // TODO(corentin@ki-dour.org) handle error.

    *module_builder = (jg_module_builder) {
        .allocator = context_builder->allocator,
        .namespace = jg_copy_identifier(allocator, namespace),
        .first_class = NULL,
        .next_module = context_builder->first_module
    };

    context_builder->first_module = module_builder;

    return module_builder;
}

void jg_context_free(jg_context* context) {
    JG_ASSERT(context != NULL);

    free(context->pool.modules);
    free(context->pool.classes);
    free(context->pool.indexes);
    free(context->pool.members);
    free(context->pool.ids);
    free(context);
}

static jg_class* get_class(const jg_context* context, const char* namespace, const char* id);

const jg_class* jg_context_get_class(const jg_context* context, const char* namespace, const char* id) {
    return get_class(context, namespace, id);
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

void jg_set_error_handler(jg_context_builder* context_builder, jg_error_handler handler) {
    JG_ASSERT(context_builder != NULL);
    if(handler == NULL) {
        handler = default_error_handler;
    }
    context_builder->error_handler = handler;
}

static void default_error_handler(jg_error_code error_code, const char* message) {
    JG_ASSERT(message != NULL);
    printf("JG ERROR : %i : %s", error_code, message);
}

static jg_pool create_pool(jg_context_builder* context_builder) {
    JG_ASSERT(context_builder != NULL);

    size_t module_count = 0;
    size_t class_count = 0;
    size_t member_count = 0;
    size_t id_length = 0;

    jg_module_builder* module_builder = context_builder->first_module;

    while(module_builder) {
        ++module_count;
        id_length += strlen(module_builder->namespace) + 1; // for terminal \0
        jg_class_builder* class_builder = module_builder->first_class;

        while(class_builder) {
            ++class_count;
            JG_ASSERT(strlen(class_builder->id) < JG_MAX_IDENTIFIER_LENGTH);
            id_length += strlen(class_builder->id) + 1; // for terminal \0
            id_length += strlen(class_builder->id) + 1; // for terminal \0
            jg_member_builder* member_builder = class_builder->first_member;
            while(member_builder) {
                ++member_count;
                JG_ASSERT(strlen(member_builder->id) < JG_MAX_IDENTIFIER_LENGTH);
                id_length += strlen(member_builder->id) + 1; // for terminal \0
                member_builder = member_builder->next_member;
            }
            class_builder = class_builder->next_class;
        }

        module_builder = module_builder->next_module;
    }

    // TODO(corentin@ki-dour.org): handle errors. (Will crash if we try to create an empty module, or empty classes).
    JG_ASSERT(module_count > 0);
    JG_ASSERT(class_count > 0);
    JG_ASSERT(member_count > 0);
    JG_ASSERT(id_length > 0);

    jg_pool result = {
        .modules = calloc(module_count, sizeof(jg_module)),
        .classes = calloc(class_count, sizeof(jg_class)),
        .indexes = calloc(module_count + class_count + member_count, sizeof(char*)),
        .members = calloc(member_count, sizeof(jg_member)),
        .ids = calloc(id_length, sizeof(char))
    };

    // TODO(corentin@ki-dour.org): handle error.
    JG_ASSERT(result.modules != NULL);
    JG_ASSERT(result.classes != NULL);
    JG_ASSERT(result.indexes != NULL);
    JG_ASSERT(result.members != NULL);
    JG_ASSERT(result.ids != NULL);

    return result;
}

static jg_context* build_context(jg_context_builder* context_builder) {
    JG_ASSERT(context_builder != NULL);

    jg_pool pool = create_pool(context_builder);
    jg_pool pool_copy = pool; // we keep the pool's buffers index copy, as jg_class_build_index will increment them.

    jg_index module_index = jg_module_build_index(context_builder->first_module, &pool);

    jg_context* context = malloc(sizeof(jg_context));
    JG_ASSERT(context != NULL); // TODO(corentin@ki-dour.org) handle error.
    *context = (jg_context) {
        .module_index = module_index,
        .pool = pool_copy,
        .error_handler = context_builder->error_handler
    };

    jg_module_builder* current_module = context_builder->first_module;
    while(current_module) {
        const char* child_namespace = current_module->namespace;

        const jg_class_builder* current_builder = current_module->first_class;
        while(current_builder) {
            const char* parent_id = current_builder->parent_id;

            if(parent_id != NULL) {
                const char* parent_namespace = current_builder->parent_namespace;
                const char* child_id = current_builder->id;
                JG_ASSERT(parent_namespace != NULL);

                jg_class* child = get_class(context, child_namespace, child_id);
                jg_class* parent = get_class(context, parent_namespace, parent_id);
                
                JG_ASSERT(child->parent == NULL);
                JG_ASSERT(parent != NULL);
                child->parent = parent;
            }

            current_builder = current_builder->next_class;
        }

        current_module = current_module->next_module;
    }

    jg_arena_free(context_builder->allocator);

    return context;
}

static jg_class* get_class(const jg_context* context, const char* namespace, const char* id) {
    JG_ASSERT(context != NULL);
    JG_ASSERT(namespace != NULL && strlen(namespace) > 0);
    JG_ASSERT(id != NULL && strlen(id) > 0);

    int module_id = jg_index_search(&context->module_index, namespace);
    if(module_id == -1) {
        return NULL;
    }

    return jg_module_get_class(&context->pool.modules[module_id], id);
}
