/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * Implementation of the void model.
 * 
 * TODO: Finish documentation here.
 *
 */
#pragma once
#include <stddef.h>

#include <uiligi/class.h>

typedef struct _UlgArena UlgArena;
typedef struct _UlgMemberDefinition UlgMemberDefinition;

struct _UlgClassDefinition {
    UlgArena* allocator;
    uintptr_t id;
    uintptr_t parent_id;
    char* name;
    UlgClassDefinition* next;
    UlgMemberDefinition* properties;
    size_t nb_members;
    size_t names_length;
};

struct _UlgClass {
    uintptr_t id;
    uintptr_t parent_id;
    char* name;
};

struct _UlgMemberDefinition {
    char* name;
    UlgGetter getter;
    UlgSetter setter;
    UlgMemberDefinition* next;
};

struct _UlgMember {
    const char* name;
    UlgGetter getter;
    UlgSetter setter;
};
