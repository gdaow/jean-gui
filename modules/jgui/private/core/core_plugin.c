/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include <jgui/core/context.h>
#include <jgui/object-model/value.h>
#include <jgui/object-model/class.h>
#include <jgui/core/core.h>

const char* jg_core_ns = "https://ki-dour.org/jean-gui/core";

#include "core_plugin.h"
#include "context.h"

static jg_value initialize(jg_arguments* arguments) {
    (void)arguments;
    return jg_none();
}

jg_id jg_core_class_id(const char* class_name) {
    return jg_id_new("https://ki-dour.org/jean-gui/core", class_name);
}

jg_id jg_object_class_id() { return jg_core_class_id("Object"); }

void jgui_core_plugin(jg_context* context) {
    jg_class* object_class = jg_context_add_class(context, jg_object_class_id(), (jg_id){0}, 0);
    jg_class_add_method(object_class, "initialize", initialize);
}
