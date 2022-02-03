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
#ifndef JGUI_PUBLIC_JGUI_CLASS_H
#define JGUI_PUBLIC_JGUI_CLASS_H

#include <jgui/misc/utils.h>
#include <jgui/object-model/member_types.h>

jg_forward_declare(jg_class)
jg_forward_declare(jg_arguments)

void jg_class_add_method(jg_class* class_, const char* name, jg_method method);

void jg_class_add_property(jg_class* class_, const char* name, jg_getter getter, jg_setter setter);

#endif
