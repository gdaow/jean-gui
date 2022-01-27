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

#include <jg/object.h>
#include <jg/template.h>
#include <jg/value.h>

#include "private/memory.h"
#include "private/stack.h"

typedef struct jg_property_template_s jg_property_template;
typedef struct jg_node_template_s jg_node_template;

typedef enum  {
    JG_TEMPLATE_PROPERTY_SCALAR,
    JG_TEMPLATE_PROPERTY_OBJECT
} jg_property_type;

struct jg_property_template_s {
    const char* name;
    jg_property_type type;
    union {
        jg_node_template* object_;
        const char* scalar;
    } value;
    jg_property_template* next_property;
};

struct jg_node_template_s {
    const jg_class* class_;
    jg_property_template* first_property;
};

struct jg_template_s {
    jg_arena* allocator;
    jg_module* module;
    jg_node_template* root_node;
};

static void parse_yaml(jg_template* template_, const char* source);
jg_template* jg_template_from_string(const char* source, jg_module* module) {
    jg_arena* allocator = jg_arena_new(0);
    jg_template* template = jg_arena_alloc(allocator, sizeof(jg_template), alignof(jg_template));
    *template = (jg_template) {
        .allocator = allocator,
        .module = module,
        .root_node = NULL
    };

    parse_yaml(template, source);
    return template;
}

void jg_template_free(jg_template* template_) {
    free(template_->allocator);
}

static void* instanciate_node(const jg_node_template* node);
void* jg_template_instanciate(const jg_template* template_) {
    return instanciate_node(template_->root_node);
}

static void* instanciate_node(const jg_node_template* node) {
    void* object = jg_object_new(node->class_);
    jg_property_template* property = node->first_property;
    while(property) {
        jg_value property_value;
        switch(property->type) {
            case JG_TEMPLATE_PROPERTY_SCALAR:
                property_value = jg_string(property->value.scalar);
                break;
            case JG_TEMPLATE_PROPERTY_OBJECT:
                {
                    void* child_object = instanciate_node(property->value.object_);
                    property_value = jg_object(child_object);
                }
                break;
            default:
                assert(false);
        }
        jg_object_set(object, property->name, property_value);
        property = property->next_property;
    }

    return object;
}


typedef struct jg_template_builder_s {
    enum {
        JG_OBJECT_STATE,
        JG_PROPERTY_STATE,
    } state;
    jg_stack node_stack;
    jg_arena* allocator;
    jg_module* module;
} jg_template_builder;


static jg_node_template* top_node(jg_template_builder* builder) {
    jg_node_template** node_ref = jg_stack_peek(&builder->node_stack);
    return *node_ref;
}

static jg_property_template* top_property(jg_template_builder* builder) {
    jg_node_template* node = top_node(builder);
    return node->first_property;
}

static void start_object(jg_template_builder* builder, const char* tag) {
    jg_arena* allocator = builder->allocator;
    jg_node_template* new_node = jg_arena_alloc(allocator, sizeof(jg_node_template), alignof(jg_node_template));

    const jg_class* class_ = jg_class_get(builder->module, tag + 1);
    *new_node = (jg_node_template) {
        .first_property = NULL,
        .class_ = class_
    };

    jg_node_template** top_node = jg_stack_peek(&builder->node_stack);
    if(*top_node == NULL) {
        *top_node = new_node;
    }
    else {
        jg_property_template* property = top_property(builder);
        property->type = JG_TEMPLATE_PROPERTY_OBJECT;
        property->value.object_ = new_node;
    }

    jg_stack_push(&builder->node_stack, &new_node);
    builder->state = JG_OBJECT_STATE;
}

static void start_property(jg_template_builder* builder, const char* tag) {
    jg_arena* allocator = builder->allocator;

    jg_property_template* new_property = jg_arena_alloc(
        allocator,
        sizeof(jg_property_template),
        alignof(jg_property_template)
    );

    jg_node_template* node = top_node(builder);
    *new_property = (jg_property_template) {
        .name = jg_arena_strcpy(allocator, tag, 1024),
        .next_property = node->first_property,
    };

    node->first_property = new_property;
    builder->state = JG_PROPERTY_STATE;
}

static void start_element(jg_template_builder* builder, const char* tag) {
    switch(builder->state) {
        case JG_PROPERTY_STATE:
          start_object(builder, tag);
          break;
        case JG_OBJECT_STATE:
          start_property(builder, tag);
          break;
    }    
}

static void scalar(jg_template_builder* builder, const char* value) {
    assert(builder->state == JG_PROPERTY_STATE);
    jg_property_template* property = top_property(builder);
    property->type = JG_TEMPLATE_PROPERTY_SCALAR;
    property->value.scalar = jg_arena_strcpy(builder->allocator, value, 1024);
}

static void end_element(jg_template_builder* builder) {
    switch(builder->state) {
        case JG_PROPERTY_STATE:
          builder->state = JG_OBJECT_STATE;
          break;
        case JG_OBJECT_STATE:
          jg_stack_pop(&builder->node_stack);
          builder->state = JG_PROPERTY_STATE;
          break;
    }    
}

typedef struct {
    jg_template_builder* builder;
    char scalar_tag[1024];
    const char* has_previous_tag;
    jg_stack tag_stack;
} jg_yaml_state;


void yaml_start_event(jg_yaml_state* state, const unsigned char* tag) {
    jg_stack* tag_stack = &state->tag_stack;
    const char* scalar_tag = state->scalar_tag;
    jg_template_builder* builder = state->builder;

    if(state->has_previous_tag) {
        start_element(builder, scalar_tag);
    }
    if(tag) {
        start_element(builder, (const char*)tag);
    }
    jg_stack_push(tag_stack, &state->has_previous_tag);
    jg_stack_push(tag_stack, &tag);
    state->has_previous_tag = NULL;
}

void yaml_scalar_event(
    jg_yaml_state* state,
    const unsigned char* value,
    const unsigned char* tag
) {
    const char* scalar_tag = state->scalar_tag;
    jg_template_builder* builder = state->builder;

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
        state->has_previous_tag = NULL;
    }
    else {
        // store the scalar value as the tag name for the next element
        strcpy(state->scalar_tag, (const char*)value);
        state->has_previous_tag = (const char*)1;
    }
}


void yaml_end_event(jg_yaml_state* state) {
    jg_stack* tag_stack = &state->tag_stack;
    jg_template_builder* builder = state->builder;

    assert(!state->has_previous_tag); // previous scalar value encountered, using it as tag

    if(*(void**)jg_stack_pop(tag_stack)) {
        end_element(builder);
    }
    if(*(void**)jg_stack_pop(tag_stack)) {
        end_element(builder);
    }

    state->has_previous_tag = NULL;
}


static void parse_yaml(jg_template* template_, const char* source) {
    yaml_parser_t parser;
    yaml_event_t event;

    jg_template_builder builder = {
        .allocator = template_->allocator,
        .module = template_->module,
        .state = JG_PROPERTY_STATE
    };
    jg_stack_init(&builder.node_stack, sizeof(jg_node_template*));
    jg_stack_push(&builder.node_stack, &template_->root_node);

    jg_yaml_state state = {
        .builder = &builder,
        .has_previous_tag = NULL
    };

    jg_stack_init(&state.tag_stack, sizeof(const char*));
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

    template_->root_node = *(void**)jg_stack_pop(&builder.node_stack);
    jg_stack_clean(&state.tag_stack);
    jg_stack_clean(&builder.node_stack);
    yaml_parser_delete(&parser);
}
