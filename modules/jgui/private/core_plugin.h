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
#ifndef JGUI_PRIVATE_CORE_PLUGIN_H
#define JGUI_PRIVATE_CORE_PLUGIN_H

typedef struct jg_context_builder_s jg_context_builder;

/**
 * Base functionalities of Jean GUI.
 */
void jgui_core_plugin(jg_context_builder* context_builder);

#endif
