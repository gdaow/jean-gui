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
#ifndef JGUI_PRIVATE_CLASS_H
#define JGUI_PRIVATE_CLASS_H

#include <jgui/class.h>

#include "common/arena.h"
#include "common/index.h"

typedef struct jg_context_s jg_context;
typedef struct jg_module_builder_s jg_module_builder;
typedef struct jg_member_builder_s jg_member_builder;
typedef struct jg_pool_s jg_pool;

struct jg_class_builder_s {
    jg_allocator* allocator;
    const char* id;
    char* parent_id;
    const char* parent_namespace;
    size_t size;
    jg_class_builder* next_class;
    jg_member_builder* first_member;
    jg_constructor constructor;
    jg_destructor destructor;
};

typedef enum {
    JG_MEMBER_PROPERTY,
    JG_MEMBER_METHOD
} jg_member_type;

typedef union {
    struct {
        jg_getter_t getter;
        jg_setter_t setter;
    } property;
    jg_method method;
} jg_member_data;

typedef struct jg_member_builder_s {
    const char* id;
    jg_member_type type;
    jg_member_data data;
    jg_member_builder* next_member;
} jg_member_builder;

typedef struct jg_member_s {
    jg_member_type type;
    jg_member_data data;
} jg_member;

struct jg_class_s {
    jg_index member_index;
    const jg_class* parent;
    jg_member* member_array;
    jg_constructor constructor;
    jg_destructor destructor;
    size_t size;
};

struct jg_arguments_s {
    jg_context* context;
    const char* method_name;
    jg_value* start;
    jg_value* end;
    bool has_error;
};

jg_index jg_class_build_index(const jg_class_builder* first_class, jg_pool* pool);

const jg_member* jg_class_get_member(const jg_class* class_, const char* id, jg_member_type type);

#endif
