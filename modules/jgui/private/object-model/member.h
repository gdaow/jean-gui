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
#ifndef JGUI_PRIVATE_OBJECT_MODEL_MEMBER_H
#define JGUI_PRIVATE_OBJECT_MODEL_MEMBER_H

#include <jgui/object-model/member.h>

#include <jgui/object-model/member_types.h>
#include <jgui/misc/utils.h>

jg_forward_declare(jg_member)

void jg_property_init(jg_member* member, jg_getter getter, jg_setter setter);

void jg_method_init(jg_member* member, jg_method method);

void jg_member_cleanup(void* member);

#endif
