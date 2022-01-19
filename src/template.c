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

typedef struct _UlgPropertyTemplate UlgPropertyTemplate;

typedef enum  {
    ULG_TEMPLATE_PROPERTY_SCALAR,
    ULG_TEMPLATE_PROPERTY_OBJECT
} UlgPropertyTemplateType;

struct _UlgPropertyTemplate {
    const char* name;
    UlgPropertyTemplateType type;
    union {
        UlgTemplate* template;
        const char* scalar;
    } value;
    UlgPropertyTemplate* next;
};

struct _UlgTemplate {
    UlgContext* context; // Could be nice to remove that
    const UlgClass* class_;
    UlgPropertyTemplate* properties;
};

UlgTemplate* ulg_template_new(UlgContext* context, const UlgClass* class_) {
    return memcpy(
        malloc(sizeof(UlgTemplate)),
        &(UlgTemplate) {
            .context = context,
            .class_ = class_,
            .properties = NULL
        },
        sizeof(UlgTemplate)
    );
}

UlgTemplate* ulg_template_new_by_name(UlgContext* context, const char* class_name) {
    const UlgClass* class_ = ulg_class_get_by_name(context, class_name);
    assert(class_);
    return ulg_template_new(context, class_);
}

void ulg_template_free(UlgTemplate* template) {
    UlgPropertyTemplate* property = template->properties;
    while(property) {
        UlgPropertyTemplate* next = property->next;
        switch(property->type) {
            case ULG_TEMPLATE_PROPERTY_OBJECT:
                ulg_template_free(property->value.template);
                break;
            default:
                break;
        }
        free(property);
        property = next;
    }
    free(template);
}

void ulg_template_set_scalar(UlgTemplate* template, const char* property_name, const char* value) {
    /* TODO: We should get the UlgProperty here and convert the value to an UlgProperty,
       as both property_name and value could be destroyed later when instanciating the template,
       and it'd be more efficient.
    */
    UlgPropertyTemplate* new_property = memcpy(
        malloc(sizeof(UlgPropertyTemplate)),
        &(UlgPropertyTemplate) {
            .name = property_name,
            .value.scalar = value,
            .next = template->properties
        },
        sizeof(UlgPropertyTemplate)
    );

    template->properties = new_property;
}

UlgTemplate* ulg_template_set_child(UlgTemplate* template, const char* property_name, const char* class_name) {
    UlgContext* context = template->context;

    const UlgClass* child_class = ulg_class_get_by_name(context, class_name);
    assert(child_class); // TODO: Handle errors.

    UlgTemplate* child_template = ulg_template_new(context, child_class);

    UlgPropertyTemplate* new_property = memcpy(
        malloc(sizeof(UlgPropertyTemplate)),
        &(UlgPropertyTemplate) {
            .name = property_name,
            .type = ULG_TEMPLATE_PROPERTY_OBJECT,
            .value.template = child_template,
            .next = template->properties
        },
        sizeof(UlgPropertyTemplate)
    );

    template->properties = new_property;
    return child_template;
}

void* ulg_template_instanciate(const UlgTemplate* template) {
    void* object = ulg_object_new(template->class_);
    UlgPropertyTemplate* property = template->properties;
    while(property) {
        UlgValue property_value;
        switch(property->type) {
            case ULG_TEMPLATE_PROPERTY_SCALAR:
                property_value = ulg_string(property->value.scalar);
                break;
            case ULG_TEMPLATE_PROPERTY_OBJECT:
                void* child_object = ulg_template_instanciate(property->value.template);
                property_value = ulg_object(child_object);
                break;
            default:
                assert(false);
        }
        ulg_object_set(object, property->name, property_value);
        property = property->next;
    }

    return object;
}
