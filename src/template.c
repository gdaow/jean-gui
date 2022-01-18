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

struct _UlgPropertyTemplate {
    const char* name;
    const char* value;
    UlgPropertyTemplate* next;
};

struct _UlgTemplate {
    UlgContext* context; // Could be nice to remove that
    const UlgClass* class_;
    UlgPropertyTemplate* properties;
};

UlgTemplate* ulg_template_new(UlgContext* context, const char* class_name) {
    const UlgClass* class_ = ulg_class_get_by_name(context, class_name);
    assert(class_);
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

void ulg_template_free(UlgTemplate* template) {
    UlgPropertyTemplate* property = template->properties;
    while(property) {
        UlgPropertyTemplate* next = property->next;
        free(property);
        property = next;
    }
    free(template);
}

void ulg_template_set_scalar(UlgTemplate* template, const char* property_name, const char* value) {
    UlgPropertyTemplate* new_property = memcpy(
        malloc(sizeof(UlgPropertyTemplate)),
        &(UlgPropertyTemplate) {
            .name = property_name,
            .value = value,
            .next = template->properties
        },
        sizeof(UlgPropertyTemplate)
    );

    template->properties = new_property;
}

void* ulg_template_instanciate(UlgTemplate* template) {
    void* object = ulg_object_new(template->class_);
    UlgPropertyTemplate* property = template->properties;
    while(property) {
        ulg_object_set(object, property->name, ulg_string(property->value));
        property = property->next;
    }

    return object;
}
