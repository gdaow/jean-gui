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
typedef void (*jg_error_handler)(jg_error_code error_code, const char* message);

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
 * Report an error.
 */
void jg_error(const jg_context* context, jg_error_code error_code, const char* format, ...);

/**
 * Set the error handler for the given context.
 */
void jg_set_error_handler(jg_context* context, jg_error_handler handler);

#endif
