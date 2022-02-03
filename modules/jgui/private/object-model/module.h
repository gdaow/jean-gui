/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 * TODO:
 *  - document.
 */
#ifndef JGUI_PRIVATE_OBJECT_MODEL_MODULE_H
#define JGUI_PRIVATE_OBJECT_MODEL_MODULE_H

#include <stddef.h>

#include <jgui/misc/utils.h>
#include <jgui/core/id.h>

jg_forward_declare(jg_class)
jg_forward_declare(jg_module)

void jg_module_init(jg_module* module);
void jg_module_cleanup(jg_module* module);

jg_class* jg_module_add_class(
    jg_module* module,
    const char* name,
    const jg_class* parent,
    size_t size
);

const jg_class* jg_module_get_class(const jg_module* module, const char* name);

#endif
