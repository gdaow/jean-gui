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
#ifndef JGUI_PUBLIC_JGUI_CONTEXT_H
#define JGUI_PUBLIC_JGUI_CONTEXT_H

typedef enum {
    JG_ERROR_NONE,
    JG_ERROR_CLASS_UNDEFINED,
    JG_ERROR_LAST
} jg_error_code;

typedef struct jg_context_s jg_context;
typedef struct jg_class_s jg_class;
typedef struct jg_context_definition_s jg_context_definition;
typedef struct jg_module_definition_s jg_module_definition;
typedef void (*jg_error_handler)(jg_error_code error_code, const char* message);
typedef void (*jg_plugin)(jg_context_definition*);

/**
 * Create a new context from a plugin array
 */
jg_context* jg_context_load(jg_plugin* plugins);

/**
 * Add a class module to a context, with the given namespace.
 */
jg_module_definition* jg_context_add_module(jg_context_definition* context_definition, const char* namespace);

/**
 * Create a new context.
 *
 * @return A pointer to the created context on sucess, or NULL if OOM.
 */
jg_context* jg_context_new();

/**
 * Frees the given context.
 */
void jg_context_free(jg_context* context);

/**
 * Get a class from it's tag and namespace.
 */
const jg_class* jg_context_get_class(const jg_context* context, const char* namespace, const char* id);

/**
 * Report an error.
 */
void jg_error(const jg_context* context, jg_error_code error_code, const char* format, ...);

/**
 * Set the error handler for the given context.
 */
void jg_set_error_handler(jg_context_definition* context_definition, jg_error_handler handler);

#endif
