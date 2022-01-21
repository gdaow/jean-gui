/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <assert.h>
#include <stdalign.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <uiligi/class.h>
#include <uiligi/module.h>
#include <uiligi/object.h>

#include "private/class.h"
#include "private/memory.h"

void ulg_class_define(UlgClassDefinition* class_, const char* name, UlgClassDeclaration parent) {
    UlgArena* allocator = class_->allocator;
    class_->name = ulg_arena_strcpy(allocator, name, MAX_NAME_LENGTH);
    class_->parent_id = (uintptr_t)parent;
}

void ulg_class_add_property(UlgClassDefinition* class_, const char* name, UlgGetter getter, UlgSetter setter) {
    class_->properties = ulg_arena_alloc_struct(
        class_->allocator,
        &(UlgMemberDefinition) {
            .next = class_->properties,
            .getter = getter,
            .setter = setter,
            .name = ulg_arena_strcpy(class_->allocator, name, MAX_NAME_LENGTH)
        }
    );
}
