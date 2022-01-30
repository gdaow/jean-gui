/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include <jgui/context.h>
#include <jgui/module.h>
#include <jgui/value.h>
#include <jgui/class.h>
#include <jgui/core.h>

const char* jg_core_ns = "https://ki-dour.org/jean-gui/core";

#include "core_plugin.h"
#include "context.h"

static jg_value initialize(jg_value* arguments) {
    (void)arguments;
    return jg_none();
}

void jgui_core_plugin(jg_context_builder* context_builder) {
    jg_module_builder* core_module = jg_context_add_module(context_builder, jg_core_ns);
    jg_class_builder* object_class = jg_module_add_class(core_module, "Object", NULL, NULL, 0);
    jg_class_add_method(object_class, "initialize", initialize);
}
