/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include "jgui/private/memory/fixed_pool.h"

#include <jgui/static_config.h>

#if JG_MEMORY_DEBUG
static const char disabled_fixed_pool_location;
const void* disabled_fixed_pool = &disabled_fixed_pool_location;
#endif
