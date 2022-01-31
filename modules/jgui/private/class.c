/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <stdalign.h>
#include <stdbool.h>

#include <jgui/context.h>
#include <jgui/debug.h>
#include <jgui/value.h>

#include "common/constants.h"
#include "common/index.h"
#include "common/memory.h"
#include "common/string.h"
#include "class.h"
#include "context.h"

void jg_class_set_constructor(jg_class_builder* class_builder, jg_constructor constructor, jg_destructor destructor) {
    class_builder->constructor = constructor;
    class_builder->destructor = destructor;
}

// Add a previously constructed member definition to a class definition.
static void add_member(
    jg_class_builder* class_builder,
    const char* id,
    const jg_member_builder* member_builder
);

void jg_class_add_method(jg_class_builder* class_builder, const char* id, jg_method method) {
    assert(class_builder != NULL);
    assert(id != NULL);
    assert(method != NULL);

    add_member(class_builder, id, &(jg_member_builder) {
        .type = JG_MEMBER_METHOD,
        .data.method = method
    });
}

void jg_class_add_property(
    jg_class_builder* class_builder,
    const char* id,
    jg_getter_t getter,
    jg_setter_t setter
) {
    assert(class_builder != NULL);
    assert(id != NULL);
    assert(getter != NULL);
    assert(setter != NULL);

    add_member(class_builder, id, &(jg_member_builder) {
        .type = JG_MEMBER_PROPERTY,
        .data.property.getter = getter,
        .data.property.setter = setter
    });
}

static jg_value pop(jg_arguments* arguments, bool (*check_type)(jg_value)) {
    jg_value* argument = arguments->start;
    if(argument == arguments->end) {
        jg_error(
            arguments->context,
            JG_ERROR_MISSING_ARGUMENT,
            "Missing argument when calling method %s", arguments->method_name
        );
        arguments->has_error = true;
        return jg_none();
    }

    if(!check_type(*argument)) {
        //TODO(corentin@ki-dour.org) Improve error feedback by giving expected and actual
        //argument.
        jg_error(
            arguments->context,
            JG_ERROR_UNEXPECTED_ARGUMENT,
            "Got argument of the wrong type when calling method %s", arguments->method_name
        );
        arguments->has_error = true;
        return jg_none();
    }

    ++arguments->start;

    return *argument;
}

bool jg_pop_bool(jg_arguments* arguments) {
    jg_value arg = pop(arguments, jg_is_bool);
    if(jg_is_none(arg)) {
        return false;
    }
    return jg_to_bool(arg);
}

int jg_pop_int(jg_arguments* arguments) {
    jg_value arg = pop(arguments, jg_is_int);
    if(jg_is_none(arg)) {
        return 0;
    }
    return jg_to_int(arg);
}

double jg_pop_double(jg_arguments* arguments) {
    jg_value arg = pop(arguments, jg_is_double);
    if(jg_is_none(arg)) {
        return .0;
    }
    return jg_to_double(arg);
}

const char* jg_pop_string(jg_arguments* arguments) {
    jg_value arg = pop(arguments, jg_is_string);
    if(jg_is_none(arg)) {
        return NULL;
    }
    return jg_to_string(arg);
}

const void* jg_pop_raw(jg_arguments* arguments) {
    jg_value arg = pop(arguments, jg_is_raw);
    if(jg_is_none(arg)) {
        return NULL;
    }
    return jg_to_raw(arg);
}

void* jg_pop_object(jg_arguments* arguments) {
    jg_value arg = pop(arguments, jg_is_object);
    if(jg_is_none(arg)) {
        return NULL;
    }
    return jg_to_object(arg);
}

bool jg_arguments_error(jg_arguments* arguments) {
    return arguments->has_error;
}

static const char* get_class_id_and_next(const void** item);
static void build_class(const void* item, int sorted_id, void* user_data);

jg_index jg_class_build_index(const jg_class_builder* first_class_builder, jg_pool* pool) {
    assert(first_class_builder != NULL);
    assert(pool != NULL);

    jg_index class_index = jg_index_build(
        first_class_builder,
        get_class_id_and_next,
        build_class,
        &pool->indexes,
        &pool->ids,
        pool
    );

    pool->classes += class_index.count;

    return class_index;
}

const jg_member* jg_class_get_member(const jg_class* class_, const char* id, jg_member_type type) {
    assert(class_ != NULL);
    assert(id != NULL);

    while(class_) {
        int member_id = jg_index_search(&(class_->member_index), id);

        if(member_id >= 0) {
            jg_member* candidate = &(class_->member_array[member_id]);
            if(candidate->type == type) {
                return candidate;
            }

            assert(false);// TODO(corentin@ki-dour.org): handle error
        }
        class_ = class_->parent;
    }

    assert(false);// TODO(corentin@ki-dour.org): handle error
    return NULL;
}

static void add_member(
    jg_class_builder* class_builder,
    const char* id,
    const jg_member_builder* member_builder
) {
    assert(class_builder != NULL);
    assert(id != NULL);
    assert(member_builder != NULL);

    jg_member_builder* new_member = jg_allocate_aligned(
        class_builder->allocator,
        sizeof(jg_member_builder),
        alignof(jg_member_builder)
    );
    assert(new_member != NULL); // TODO(corentin@ki-dour.org) handle error.

    *new_member = *member_builder;
    new_member->id = jg_copy_identifier(class_builder->allocator, id);
    assert(new_member->id != NULL); // TODO(corentin@ki-dour.org) handle error.
    new_member->next_member = class_builder->first_member;
    class_builder->first_member = new_member;
}

static const char* get_member_id_and_next(const void** item) {
    assert(item != NULL);
    assert(*item != NULL);

    const jg_member_builder* member_builder = *item;
    const char* id = member_builder->id;
    *item = member_builder->next_member;
    return id;
}

static void build_member(const void* item, int sorted_id, void* user_data) {
    assert(item != NULL);
    assert(sorted_id >= 0);
    assert(user_data != NULL);

    const jg_member_builder* member_builder = item;
    jg_pool* pool = user_data;
    pool->members[sorted_id] = (jg_member) {
        .type = member_builder->type,
        .data = member_builder->data
    };
}

static const char* get_class_id_and_next(const void** item) {
    assert(item != NULL);
    assert(*item != NULL);

    const jg_class_builder* class_builder = *item;
    const char* id = class_builder->id;
    *item = class_builder->next_class;
    return id;
}

static jg_index build_member_index(const jg_member_builder* first_member, jg_pool* pool) {
    assert(pool != NULL);

    return jg_index_build(
        first_member,
        get_member_id_and_next,
        build_member,
        &pool->indexes,
        &pool->ids,
        pool
    );
}

static void build_class(const void* item, int sorted_id, void* user_data) {
    assert(item != NULL);
    assert(sorted_id >= 0);
    assert(user_data != NULL);

    jg_pool* pool = user_data;
    jg_class* class_pool = pool->classes;
    const jg_class_builder* class_builder = item;

    jg_class* new_class = &class_pool[sorted_id];

    *new_class = (jg_class) {
        .size = class_builder->size,
        .member_index = build_member_index(class_builder->first_member, pool),
        .member_array = pool->members,
        .constructor = class_builder->constructor,
        .destructor = class_builder->destructor
    };

    pool->members += new_class->member_index.count;
}
