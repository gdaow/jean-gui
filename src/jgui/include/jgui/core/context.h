/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * jg_context is the root object of Jean-Gui runtime. Nothing is declared
 * through statics, so context holds classes declaration, error handling
 * methods end everything that should be globally accessible in through
 * Jean-Gui.
 *
 */
#ifndef JGUI_CORE_CONTEXT_H
#define JGUI_CORE_CONTEXT_H

#include <stddef.h>

#include <jgui/core/error-code.h>
#include <jgui/core/id.h>
#include <jgui/misc/utils.h>

jg_forward_declare(jg_class);
jg_forward_declare(jg_module);

typedef struct jg_context_s jg_context;

/**
 * Error handler method type.
 *
 * @param error_code Error code enum.
 * @param message    Human-readable error description.
 *
 */
typedef void (*jg_error_handler)(jg_error_code error_code, const char* message);

/**
 * Callback feedable to jg_context_load, allowing to extend context when creating
 * it (adding new classes).
 *
 * @param context The loading context to extend
 */
typedef void (*jg_plugin)(jg_context*);

/**
 * Create a new context from a plugin array
 *
 * The context will automatically include the jg_core_plugin, providing basic
 * Jean-Gui functionalities (base object type for example).
 *
 * @param plugins NULL-terminated array of plugin callback, that will be called
 *                sequencially with the newly created context.
 */
jg_context* jg_context_load(jg_plugin* plugins);

/**
 * Release a context.
 */
void jg_context_free(jg_context* context);

/**
 * Add a class to a context.
 *
 * @param id The     Id of the class, (module and class name). 
 *                   Generaly use @ref jg_id_new(module_name, class_name).
 * @param parent_id  Id of the parent class, or @ref jg_id_none if the class have no parent.
 * @param size       Size of the object this new class wraps. See @ref jg_class_new.
 */
jg_class* jg_context_add_class(jg_context* context, jg_id id, jg_id parent_id, size_t size);

/**
 * Get a class from it's id.
 *
 * @param id Id of the class to retrieve.
 *
 * @return The requested class, or null if the class was not found.
 */
const jg_class* jg_context_get_class(const jg_context* context, jg_id id);

/**
 * Set the error handling callback in this context.
 *
 * @param jg_error_handler The callback that will be called when an error occurs.
 */
void jg_set_error_handler(jg_context* context, jg_error_handler handler);

/**
 * Raise an error in this context.
 * This will call the callback previously set through jg_set_error_handler, or the default
 * error handler, that will printf the message to stdout if none was set.
 *
 * @param error_code Error code enum.
 * @param format     Message format, will be passed to sprintf with the subsequent parameters.
 * @param ...        Message format parameters.
 */
void jg_error(const jg_context* context, jg_error_code error_code, const char* format, ...);

#endif

