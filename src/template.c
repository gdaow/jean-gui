/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdalign.h>
#include <string.h>

#include <yaml.h>

#include <mezo/object.h>
#include <mezo/template.h>
#include <mezo/value.h>

#include "private/memory.h"
#include "private/stack.h"

typedef struct mz_property_template_s mz_property_template;
typedef struct mz_node_template_s mz_node_template;

typedef enum  {
    MZ_TEMPLATE_PROPERTY_SCALAR,
    MZ_TEMPLATE_PROPERTY_OBJECT
} mz_property_type;

struct mz_property_template_s {
    const char* name;
    mz_property_type type;
    union {
        mz_node_template* object_;
        const char* scalar;
    } value;
    mz_property_template* next_property;
};

struct mz_node_template_s {
    const mz_class* class_;
    mz_property_template* first_property;
};

struct mz_template_s {
    mz_arena* allocator;
    mz_module* module;
    mz_node_template* root_node;
};

static void parse_yaml(mz_template* template_, const char* source);
mz_template* mz_template_from_string(const char* source, mz_module* module) {
    mz_arena* allocator = mz_arena_new(0);
    mz_template* template = MZ_ARENA_ALLOC_INIT(
        allocator,
        mz_template,
        &(mz_template) {
            .allocator = allocator,
            .module = module,
            .root_node = NULL
        }
    );
    parse_yaml(template, source);
    return template;
}

void mz_template_free(mz_template* template_) {
    free(template_->allocator);
}

static void* instanciate_node(const mz_node_template* node);
void* mz_template_instanciate(const mz_template* template_) {
    return instanciate_node(template_->root_node);
}

static void* instanciate_node(const mz_node_template* node) {
    void* object = mz_object_new(node->class_);
    mz_property_template* property = node->first_property;
    while(property) {
        mz_value property_value;
        switch(property->type) {
            case MZ_TEMPLATE_PROPERTY_SCALAR:
                property_value = mz_string(property->value.scalar);
                break;
            case MZ_TEMPLATE_PROPERTY_OBJECT:
                {
                    void* child_object = instanciate_node(property->value.object_);
                    property_value = mz_object(child_object);
                }
                break;
            default:
                assert(false);
        }
        mz_object_set(object, property->name, property_value);
        property = property->next_property;
    }

    return object;
}


typedef struct mz_template_builder_s {
    enum {
        MZ_OBJECT_STATE,
        MZ_PROPERTY_STATE,
    } state;
    mz_stack node_stack;
    mz_arena* allocator;
    mz_module* module;
} mz_template_builder;


static mz_node_template* top_node(mz_template_builder* builder) {
    mz_node_template** node_ref = mz_stack_peek(&builder->node_stack);
    return *node_ref;
}

static mz_property_template* top_property(mz_template_builder* builder) {
    mz_node_template* node = top_node(builder);
    return node->first_property;
}

static void start_object(mz_template_builder* builder, const char* tag) {
    mz_node_template** top_node = mz_stack_peek(&builder->node_stack);
    const mz_class* class_ = mz_class_get(builder->module, tag + 1);
    mz_node_template* new_node = MZ_ARENA_ALLOC_INIT(
        builder->allocator,
        mz_node_template,
        &(mz_node_template) {
            .first_property = NULL,
            .class_ = class_
        }
    );

    if(*top_node == NULL) {
        *top_node = new_node;
    }
    else {
        mz_property_template* property = top_property(builder);
        property->type = MZ_TEMPLATE_PROPERTY_OBJECT;
        property->value.object_ = new_node;
    }

    mz_stack_push(&builder->node_stack, &new_node);
    builder->state = MZ_OBJECT_STATE;
}

static void start_property(mz_template_builder* builder, const char* tag) {
    mz_arena* allocator = builder->allocator;
    mz_node_template* node = top_node(builder);

    node->first_property = MZ_ARENA_ALLOC_INIT(
        allocator,
        mz_property_template,
        &(mz_property_template) {
            .name = mz_arena_strcpy(allocator, tag, 1024, NULL),
            .next_property = node->first_property,
        }
    );
    builder->state = MZ_PROPERTY_STATE;
}

static void start_element(mz_template_builder* builder, const char* tag) {
    switch(builder->state) {
        case MZ_PROPERTY_STATE:
          start_object(builder, tag);
          break;
        case MZ_OBJECT_STATE:
          start_property(builder, tag);
          break;
    }    
}

static void scalar(mz_template_builder* builder, const char* value) {
    assert(builder->state == MZ_PROPERTY_STATE);
    mz_property_template* property = top_property(builder);
    property->type = MZ_TEMPLATE_PROPERTY_SCALAR;
    property->value.scalar = mz_arena_strcpy(builder->allocator, value, 1024, NULL);
}

static void end_element(mz_template_builder* builder) {
    switch(builder->state) {
        case MZ_PROPERTY_STATE:
          builder->state = MZ_OBJECT_STATE;
          break;
        case MZ_OBJECT_STATE:
          mz_stack_pop(&builder->node_stack);
          builder->state = MZ_PROPERTY_STATE;
          break;
    }    
}

typedef struct {
    mz_template_builder* builder;
    char scalar_tag[1024];
    const char* has_previous_tag;
    mz_stack tag_stack;
} mz_yaml_state;


void yaml_start_event(mz_yaml_state* state, const unsigned char* tag) {
    mz_stack* tag_stack = &state->tag_stack;
    const char* scalar_tag = state->scalar_tag;
    mz_template_builder* builder = state->builder;

    if(state->has_previous_tag) {
        start_element(builder, scalar_tag);
    }
    if(tag) {
        start_element(builder, (const char*)tag);
    }
    mz_stack_push(tag_stack, &state->has_previous_tag);
    mz_stack_push(tag_stack, &tag);
    state->has_previous_tag = false;
}

void yaml_scalar_event(
    mz_yaml_state* state,
    const unsigned char* value,
    const unsigned char* tag
) {
    const char* scalar_tag = state->scalar_tag;
    mz_template_builder* builder = state->builder;

    // we are reading a property value.
    if(state->has_previous_tag) {
        if(tag) {
            start_element(builder, (const char*)tag);
        }

        start_element(builder, scalar_tag);
        scalar(builder, (const char*)value);
        end_element(builder);

        if(tag) {
            end_element(builder);
        }
        state->has_previous_tag = false;
    }
    else {
        // store the scalar value as the tag name for the next element
        strcpy(state->scalar_tag, (const char*)value);
        state->has_previous_tag = true;
    }
}


void yaml_end_event(mz_yaml_state* state) {
    mz_stack* tag_stack = &state->tag_stack;
    mz_template_builder* builder = state->builder;

    assert(!state->has_previous_tag); // previous scalar value encountered, using it as tag

    if(*(void**)mz_stack_pop(tag_stack)) {
        end_element(builder);
    }
    if(*(void**)mz_stack_pop(tag_stack)) {
        end_element(builder);
    }

    state->has_previous_tag = false;
}


static void parse_yaml(mz_template* template_, const char* source) {
    yaml_parser_t parser;
    yaml_event_t event;

    mz_template_builder builder = {
        .allocator = template_->allocator,
        .module = template_->module,
        .state = MZ_PROPERTY_STATE
    };
    mz_stack_init(&builder.node_stack, sizeof(mz_node_template*));
    mz_stack_push(&builder.node_stack, &template_->root_node);

    mz_yaml_state state = {
        .builder = &builder,
        .has_previous_tag = false
    };

    mz_stack_init(&state.tag_stack, sizeof(const char*));
    yaml_parser_initialize(&parser);
    yaml_parser_set_input_string(&parser, (unsigned char *)source, strlen(source));

    while(yaml_parser_parse(&parser, &event)) {
        switch(event.type) {
            case YAML_MAPPING_START_EVENT:
                yaml_start_event(&state, event.data.mapping_start.tag);
                break;
            case YAML_SEQUENCE_START_EVENT:
                yaml_start_event(&state, event.data.sequence_start.tag);
                break;
            case YAML_SCALAR_EVENT:
                yaml_scalar_event(&state, event.data.scalar.value, event.data.scalar.tag);
                break;
            case YAML_MAPPING_END_EVENT:
            case YAML_SEQUENCE_END_EVENT:
                yaml_end_event(&state);
            default:
                break;
        }

        if(event.type == YAML_STREAM_END_EVENT) {
            break;
        }

        yaml_event_delete(&event);
    }
    yaml_event_delete(&event);

    template_->root_node = *(void**)mz_stack_pop(&builder.node_stack);
    mz_stack_clean(&state.tag_stack);
    mz_stack_clean(&builder.node_stack);
    yaml_parser_delete(&parser);
}
