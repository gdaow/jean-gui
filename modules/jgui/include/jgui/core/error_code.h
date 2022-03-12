/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * Context error code enum declaration.
 *
 */
#ifndef JGUI_INCLUDE_JGUI_ERROR_CODE_H
#define JGUI_INCLUDE_JGUI_ERROR_CODE_H

/**
 * Error code, usable in @ref jg_context_error and error hanlders set
 * through @ref jg_context_set_error_handler.
 */
typedef enum {
    /** No error occured. */
    JG_ERROR_NONE,

    /** A requested class was not found. */
    JG_ERROR_CLASS_UNDEFINED,

    /** Missing arguments during a dynamic method call. */
    JG_ERROR_MISSING_ARGUMENT,

    /** Got a wrong argument type during a method call. */
    JG_ERROR_UNEXPECTED_ARGUMENT,

    /** Last error code, for enumeration purposes. */
    JG_ERROR_LAST
} jg_error_code;

#endif

