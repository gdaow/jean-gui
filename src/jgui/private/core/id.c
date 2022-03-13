/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */
#include <stdlib.h>
#include <string.h>

#include <jgui/core/id.h>

#include <jgui/private/misc/assert.h>

const jg_id JG_ID_NULL = { .namespace_ = NULL, .name = NULL };

jg_id JG_ID(const char* namespace_, const char* name) {
    assert(namespace_ != NULL && name != NULL && strlen(namespace_) > 0 && strlen(name) > 0);
    return (jg_id) { .namespace_ = namespace_, .name = name };
}

bool jg_id_is_null(jg_id id) {
    return id.namespace_ == NULL && id.name == NULL;
}

