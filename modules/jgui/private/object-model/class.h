/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 * TODO:
 *  -document
 */
#ifndef JGUI_PRIVATE_OBJECT_MODEL_CLASS_H
#define JGUI_PRIVATE_OBJECT_MODEL_CLASS_H

#include <jgui/object-model/class.h>

#include <stddef.h>

#include "jgui/private/containers/index.h"
#include "jgui/private/misc/utils.h"

forward_declare(jg_class)
forward_declare(jg_member)

void jg_class_init(jg_class* class_, const jg_class* parent, size_t size);

const jg_member* jg_class_get_member(const jg_class* class_, const char* member_name);

void jg_class_cleanup(void* class_);

struct jg_class_s {
    jg_index member_index;
    const jg_class* parent;
    size_t size;
};

#endif

