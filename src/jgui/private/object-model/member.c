/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "jgui/private/object-model/member.h"

#include "jgui/private/misc/assert.h"

#include <jgui/object-model/member.h>

void jg_method_init(jg_member* member, jg_method method) {
	*member = (jg_member) {._data._method = method, ._type = JG_MEMBER_METHOD};
}

jg_value jg_method_call(const jg_member* method, jg_arguments* arguments) {
	assert(method->_type == JG_MEMBER_METHOD);
	return (*(method->_data._method))(arguments);
}

void jg_property_init(jg_member* member, jg_getter getter, jg_setter setter) {
	*member =
	    (jg_member) {._type = JG_MEMBER_PROPERTY,
	                 ._data._property = {._getter = getter, ._setter = setter}};
}

jg_value jg_property_get(const jg_member* property, void* object) {
	assert(property->_type == JG_MEMBER_PROPERTY);
	return (*(property->_data._property._getter))(object);
}

void jg_property_set(const jg_member* property, void* object, jg_value value) {
	assert(property->_type == JG_MEMBER_PROPERTY);
	(*(property->_data._property._setter))(object, value);
}

void jg_member_cleanup(jg_member* member) {
	(void)member;
}

/*
// Add a previously constructed member definition to a class definition.
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
        //TODO(corentin@ki-dour.org) Improve error feedback by giving expected
and actual
        //argument.
        jg_error(
            arguments->context,
            JG_ERROR_UNEXPECTED_ARGUMENT,
            "Got argument of the wrong type when calling method %s",
arguments->method_name
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

void jg_member_init(jg_member_index* index, jg_member_pool pool,
jg_named_node_index node_index) { size_t count = jg_index_size(node_index);
    jg_member_index* member_array = jg_fixed_pool_alloc(pool, count);
    const char** key

    for(size_t i = 0 ; i < count; ++i) {
        jg_named_node* node = jg_index_get(elements, i);
        jg_member_builder* builder = container_of(node, jg_member_builder,
node); char* key = jg_string_copy(string_pool, ) jg_index_set(member_index, i, )

        member_array = (jg_member) {
            .type = member_builder->type,
            .data = member_builder->data
        };

    }
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

const jg_member* jg_class_get_member(const jg_class* class_, const char* id,
jg_member_type type) { assert(class_ != NULL); assert(id != NULL);

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

static jg_index build_member_index(const jg_member_builder* first_member,
jg_pool* pool) { assert(pool != NULL);

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

*/
