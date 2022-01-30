/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 */
#include <jgui/wx/common.h>

JWX_HEADER_START

// forward declarations
typedef struct jg_module_builder_s jg_module_builder;

// jwx_app class declaration
typedef struct jwx_app_s jwx_app;

extern const char* jwx_app_class_id;

void jwx_app_class_register(jg_module_builder* module);

// jwx_app methods
void jwx_app_run(jwx_app* app);

JWX_HEADER_END
