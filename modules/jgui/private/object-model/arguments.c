/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <jgui/object-model/arguments.h>

#include <jgui/object-model/value.h>

jg_arguments jg_arguments_init(jg_value* arguments, size_t size) {
    (void)arguments;
    (void)size;
    return (jg_arguments) {0};
}

jg_value jg_argument_pop(jg_arguments* arguments) {
    (void)arguments;
    return jg_none();
}

bool jg_pop_bool(jg_arguments* arguments) {
    (void)arguments;
    return false;
}

int jg_pop_int(jg_arguments* arguments) {
    (void)arguments;
    return 0;
}

double jg_pop_double(jg_arguments* arguments) {
    (void)arguments;
    return .0;
}

const char* jg_pop_string(jg_arguments* arguments) {
    (void)arguments;
    return NULL;
}

void* jg_pop_raw(jg_arguments* arguments) {
    (void)arguments;
    return NULL;
}

void* jg_pop_object(jg_arguments* arguments) {
    (void)arguments;
    return NULL;
}
