/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 * TODO:
 *  - Rename to object_model plugin, and one will probably load template stuff.
 */
#ifndef JGUI_PRIVATE_CORE_PLUGIN_H
#define JGUI_PRIVATE_CORE_PLUGIN_H

#include <jgui/misc/utils.h>

jg_forward_declare(jg_context)

/**
 * Base functionalities of Jean GUI.
 */
void jgui_core_plugin(jg_context* context);

#endif
