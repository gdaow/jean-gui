/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 * Private declarations for jgui/include/object-model/class.h.
 * See public include for documentation.
 *
 */
#ifndef JGUI_PRIVATE_OBJECT_MODEL_CLASS_H
#define JGUI_PRIVATE_OBJECT_MODEL_CLASS_H

#include <jgui/object-model/class.h>

#include <stddef.h>

#include "jgui/private/containers/index.h"
#include "jgui/private/misc/utils.h"

forward_declare(jg_class)
forward_declare(jg_member)

/**
 * Initialize a class.
 *
 * @param parent Parent class, or NULL.
 * @param size   Size of object this class describes. Usually sizeof(wrapped_c_type).
 */
void jg_class_init(jg_class* class_, const jg_class* parent, size_t size);

/**
 * Retrieve a member that was previously registered in this class using
 * jg_class_add_method or jg_class_add_property. If the member is not defined on this
 * class, it will be recursively searched in parent classes.
 *
 * @param member_name Name of the member to retrieve.
 *
 * @return A pointer to the retrieved member, or NULL if the member was not found in
 *         this class or it's parents.
 */
const jg_member* jg_class_get_member(const jg_class* class_, const char* member_name);

/**
 * Release memory owned by this class instance.
 */
void jg_class_cleanup(void* class_);

/**
 * /!\ Private struct (see doc-private-struct) /!\
 */
struct jg_class_s {
    jg_index member_index;
    const jg_class* parent;
    size_t size;
};

#endif

