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

#include <mezo/object.h>
#include <mezo/template.h>
#include <mezo/value.h>

#include "private/memory.h"

typedef struct mz_property_template_s mz_property_template;

typedef enum  {
    mz_TEMPLATE_PROPERTY_SCALAR,
    mz_TEMPLATE_PROPERTY_OBJECT
} mz_property_type;

struct mz_property_template_s {
    const char* name;
    mz_property_type type;
    union {
        mz_template* template_;
        const char* scalar;
    } value;
    mz_property_template* next;
};

struct mz_template_s {
    mz_module* module; // Could be nice to remove that
    const mz_class* class_;
    mz_property_template* properties;
};

mz_template* mz_template_from_string(const char* source, mz_module* module) {
    (void)source;
    (void)module;
    return NULL;
}

void mz_template_free(mz_template* template_) {
    mz_property_template* property = template_->properties;
    while(property) {
        mz_property_template* next = property->next;
        switch(property->type) {
            case mz_TEMPLATE_PROPERTY_OBJECT:
                mz_template_free(property->value.template_);
                break;
            default:
                break;
        }
        free(property);
        property = next;
    }
    free(template_);
}

void mz_template_set_scalar(mz_template* template_, const char* property_name, const char* value) {
    /* TODO: We should get the UlgProperty here and convert the value to an UlgProperty,
       as both property_name and value could be destroyed later when instanciating the template_,
       and it'd be more efficient.
    */
    mz_property_template* new_property = memcpy(
        malloc(sizeof(mz_property_template)),
        &(mz_property_template) {
            .name = property_name,
            .value.scalar = value,
            .next = template_->properties
        },
        sizeof(mz_property_template)
    );

    template_->properties = new_property;
}

mz_template* mz_template_set_child(mz_template* template_, const char* property_name, const char* class_name) {
    mz_module* module = template_->module;

    const mz_class* child_class = mz_class_get(module, class_name);
    assert(child_class); // TODO(corenting@ki-dour.org): Handle errors.

    mz_template* child_template = NULL;

    mz_property_template* new_property = MZ_MALLOC_INIT(&(mz_property_template) {
        .name = property_name,
        .type = mz_TEMPLATE_PROPERTY_OBJECT,
        .value.template_ = child_template,
        .next = template_->properties
    });

    template_->properties = new_property;
    return child_template;
}

void* mz_template_instanciate(const mz_template* template_) {
    void* object = mz_object_new(template_->class_);
    mz_property_template* property = template_->properties;
    while(property) {
        mz_value property_value;
        switch(property->type) {
            case mz_TEMPLATE_PROPERTY_SCALAR:
                property_value = mz_string(property->value.scalar);
                break;
            case mz_TEMPLATE_PROPERTY_OBJECT:
                {
                    void* child_object = mz_template_instanciate(property->value.template_);
                    property_value = mz_object(child_object);
                }
                break;
            default:
                assert(false);
        }
        mz_object_set(object, property->name, property_value);
        property = property->next;
    }

    return object;
}
