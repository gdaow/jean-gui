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
#ifndef JGUI_INCLUDE_JGUI_CONTEXT_H
#define JGUI_INCLUDE_JGUI_CONTEXT_H

#include <stddef.h>

#include <jgui/core/id.h>
#include <jgui/core/error_code.h>
#include <jgui/misc/utils.h>

jg_forward_declare(jg_class)
jg_forward_declare(jg_module)

typedef struct jg_context_s jg_context;

typedef void (*jg_error_handler)(jg_error_code error_code, const char* message);

typedef void (*jg_plugin)(jg_context*);
/**
 * Create a new context from a plugin array
 */
jg_context* jg_context_load(jg_plugin* plugins);

/**
 * Frees the given context.
 */
void jg_context_free(jg_context* context);

/**
 * Add a class module to a context, with the given namespace.
 */
jg_class* jg_context_add_class(jg_context* context, jg_id id, jg_id parent_id, size_t size);

/**
 * Get a class from it's tag and namespace.
 */
const jg_class* jg_context_get_class(const jg_context* context, jg_id id);

void jg_set_error_handler(jg_context* context, jg_error_handler handler);

void jg_error(const jg_context* context, jg_error_code error_code, const char* format, ...);

#endif
