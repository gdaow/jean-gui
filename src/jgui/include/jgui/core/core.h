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
#ifndef JGUI_INCLUDE_JGUI_CORE_H
#define JGUI_INCLUDE_JGUI_CORE_H

#include <jgui/core/id.h>

jg_id jg_core_class_id(const char* class_name);

jg_id jg_object_class_id();

#endif
