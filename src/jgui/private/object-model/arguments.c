/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "jgui/private/misc//assert.h"

#include <jgui/object-model/arguments.h>
#include <jgui/object-model/value.h>

jg_arguments jg_arguments_init(jg_value* arguments, size_t size) {
	return (jg_arguments) {._start = arguments, ._end = arguments + size};
}

jg_value jg_argument_pop(jg_arguments* arguments) {
	jg_value* start = arguments->_start++;
	assert(start < arguments->_end);
	return *start;
}

bool jg_pop_bool(jg_arguments* arguments) {
	return jg_to_bool(jg_argument_pop(arguments));
}

int jg_pop_int(jg_arguments* arguments) {
	return jg_to_int(jg_argument_pop(arguments));
}

double jg_pop_double(jg_arguments* arguments) {
	return jg_to_double(jg_argument_pop(arguments));
}

const char* jg_pop_string(jg_arguments* arguments) {
	return jg_to_string(jg_argument_pop(arguments));
}

const void* jg_pop_raw(jg_arguments* arguments) {
	return jg_to_raw(jg_argument_pop(arguments));
}

void* jg_pop_object(jg_arguments* arguments) {
	return jg_to_object(jg_argument_pop(arguments));
}
