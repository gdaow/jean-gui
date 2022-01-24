/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <uiligi/object.h>
#include <uiligi/template.h>
#include <uiligi/value.h>

#include "private/memory.h"

typedef struct ulg_property_template_t ulg_property_template_t;

typedef enum  {
    ULG_TEMPLATE_PROPERTY_SCALAR,
    ULG_TEMPLATE_PROPERTY_OBJECT
} ulg_property_type_t;

struct ulg_property_template_t {
    const char* name;
    ulg_property_type_t type;
    union {
        ulg_template_t* template_;
        const char* scalar;
    } value;
    ulg_property_template_t* next;
};

struct ulg_template_s {
    ulg_module_t* module; // Could be nice to remove that
    const ulg_class_t* class_;
    ulg_property_template_t* properties;
};

ulg_template_t* ulg_template_from_string(const char* source, ulg_module_t* module) {
    (void)source;
    (void)module;
    return NULL;
}

void ulg_template_free(ulg_template_t* template_) {
    ulg_property_template_t* property = template_->properties;
    while(property) {
        ulg_property_template_t* next = property->next;
        switch(property->type) {
            case ULG_TEMPLATE_PROPERTY_OBJECT:
                ulg_template_free(property->value.template_);
                break;
            default:
                break;
        }
        free(property);
        property = next;
    }
    free(template_);
}

void ulg_template_set_scalar(ulg_template_t* template_, const char* property_name, const char* value) {
    /* TODO: We should get the UlgProperty here and convert the value to an UlgProperty,
       as both property_name and value could be destroyed later when instanciating the template_,
       and it'd be more efficient.
    */
    ulg_property_template_t* new_property = memcpy(
        malloc(sizeof(ulg_property_template_t)),
        &(ulg_property_template_t) {
            .name = property_name,
            .value.scalar = value,
            .next = template_->properties
        },
        sizeof(ulg_property_template_t)
    );

    template_->properties = new_property;
}

ulg_template_t* ulg_template_set_child(ulg_template_t* template_, const char* property_name, const char* class_name) {
    ulg_module_t* module = template_->module;

    const ulg_class_t* child_class = ulg_class_get(module, class_name);
    assert(child_class); // TODO(corenting@ki-dour.org): Handle errors.

    ulg_template_t* child_template = NULL;

    ulg_property_template_t* new_property = ULG_MALLOC_INIT(&(ulg_property_template_t) {
        .name = property_name,
        .type = ULG_TEMPLATE_PROPERTY_OBJECT,
        .value.template_ = child_template,
        .next = template_->properties
    });

    template_->properties = new_property;
    return child_template;
}

void* ulg_template_instanciate(const ulg_template_t* template_) {
    void* object = ulg_object_new(template_->class_);
    ulg_property_template_t* property = template_->properties;
    while(property) {
        ulg_value_t property_value;
        switch(property->type) {
            case ULG_TEMPLATE_PROPERTY_SCALAR:
                property_value = ulg_string(property->value.scalar);
                break;
            case ULG_TEMPLATE_PROPERTY_OBJECT:
                {
                    void* child_object = ulg_template_instanciate(property->value.template_);
                    property_value = ulg_object(child_object);
                }
                break;
            default:
                assert(false);
        }
        ulg_object_set(object, property->name, property_value);
        property = property->next;
    }

    return object;
}
