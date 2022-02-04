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
#ifndef JGUI_PUBLIC_MISC_UTILS_H
#define JGUI_PUBLIC_MISC_UTILS_H

#define jg_forward_declare(type)\
    typedef struct type##_s type;

#define JG_WEAK __attribute__((weak))
#endif
