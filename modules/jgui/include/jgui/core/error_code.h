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
#ifndef JGUI_INCLUDE_JGUI_ERROR_CODE_H
#define JGUI_INCLUDE_JGUI_ERROR_CODE_H

typedef enum {
    JG_ERROR_NONE,
    JG_ERROR_CLASS_UNDEFINED,

    JG_ERROR_MISSING_ARGUMENT,
    JG_ERROR_UNEXPECTED_ARGUMENT,

    JG_ERROR_LAST
} jg_error_code;

#endif
