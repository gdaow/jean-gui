/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 * jg_value wraps typed literals into a generic values, It's used with jg_object
 * dynamic getters, setters and methods.
 *
 * Only int and double numerical values are supported. Supporting several stride
 * for integers and floating-point numbers would cause the number of
 * jg_value-related functions to grow rapidely. As it is supposed to be used in
 * UI-binding related code, int and double will suffice to represent all needed
 * values, while memory usage induced by larger memory representation shouldn't
 * be an issue.
 *
 */
#ifndef JGUI_OBJECT_MODEL_VALUE_H
#define JGUI_OBJECT_MODEL_VALUE_H

#include <stdbool.h>

/**
 * /!\ Private struct (see doc-private-struct) /!\
 */
typedef struct jg_value_s {
	int _type;
	union {
		bool _bool;
		const char* _string;
		int _int;
		double _double;
		void* _raw;
		void* _object;
	} _value;
} jg_value;

/** Get a None value, used to differentiate no value from NULL */
jg_value jg_none();

/**
 * Wrap the given bool value into a generic jg_value.
 *
 * @param value Value to wrap.
 *
 * @return The wrapped bool value.
 */
jg_value jg_bool(bool value);

/**
 * Wrap the given int value into a generic jg_value.
 *
 * @param value Value to wrap.
 *
 * @return The wrapped int value.
 */
jg_value jg_int(int value);

/**
 * Wrap the given double value into a generic jg_value.
 *
 * @param value Value to wrap.
 *
 * @return The wrapped double value.
 */
jg_value jg_double(double value);

/**
 * Wrap the given string value into a generic jg_value.
 *
 * The string isn't copied, lifetime of the returned jg_value should be less
 * than the given string.
 *
 * @param value Value to wrap.
 *
 * @return The wrapped string value.
 */
jg_value jg_string(const char* value);

/**
 * Wrap the given raw pointer into a generic jg_value.
 *
 * @param value Value to wrap.
 *
 * @return The wrapped raw pointer value.
 */
jg_value jg_raw(void* value);

/**
 * Wrap the given object into a generic jg_value.
 *
 * @param value Object to wrap.
 *
 * @return The wrapped object value.
 */
jg_value jg_object(void* value);

/**
 * Check if a jg_value is the none value.
 *
 * @param value Value to check.
 *
 * @return True if the given value is none.
 */
bool jg_is_none();

/**
 * Check if a jg_value wraps a bool.
 *
 * @param value Value to check.
 *
 * @return True if the given value wraps a bool.
 */
bool jg_is_bool(jg_value value);

/**
 * Check if a jg_value wraps an int.
 *
 * @param value Value to check.
 *
 * @return True if the given value wraps a int.
 */
bool jg_is_int(jg_value value);

/**
 * Check if a jg_value wraps an double.
 *
 * @param value Value to check.
 *
 * @return True if the given value wraps a double.
 */
bool jg_is_double(jg_value value);

/**
 * Check if a jg_value wraps an string.
 *
 * @param value Value to check.
 *
 * @return True if the given value wraps a string.
 */
bool jg_is_string(jg_value value);

/**
 * Check if a jg_value wraps an raw pointer.
 *
 * @param value Value to check.
 *
 * @return True if the given value wraps a raw pointer.
 */
bool jg_is_raw(jg_value value);

/**
 * Check if a jg_value wraps an object.
 *
 * @param value Value to check.
 *
 * @return True if the given value wraps a object.
 */
bool jg_is_object(jg_value value);

/**
 * Unwrap a jg_value to a bool value.
 *
 * If the value doesn't wrap a bool, the behavior is undefined.
 *
 * @param value Value to unwrap.
 *
 * @return The unwrapped bool value.
 */
bool jg_to_bool(jg_value value);

/**
 * Unwrap a jg_value to an int value.
 *
 * If the value doesn't wrap an int, the behavior is undefined.
 *
 * @param value Value to unwrap.
 *
 * @return The unwrapped int value.
 */
int jg_to_int(jg_value value);

/**
 * Unwrap a jg_value to a double value.
 *
 * If the value doesn't wrap a double, the behavior is undefined.
 *
 * @param value Value to unwrap.
 *
 * @return The unwrapped double value.
 */
double jg_to_double(jg_value value);

/**
 * Unwrap a jg_value to a string value.
 *
 * If the value doesn't wrap a string, the behavior is undefined.
 *
 * @param value Value to unwrap.
 *
 * @return The unwrapped string value.
 */
const char* jg_to_string(jg_value value);

/**
 * Unwrap a jg_value to a raw pointer value.
 *
 * If the value doesn't wrap a raw pointer, the behavior is undefined.
 *
 * @param value Value to unwrap.
 *
 * @return The unwrapped raw pointer value.
 */
const void* jg_to_raw(jg_value value);

/**
 * Unwrap a jg_value to a object value.
 *
 * If the value doesn't wrap a object, the behavior is undefined.
 *
 * @param value Value to unwrap.
 *
 * @return The unwrapped object.
 */
void* jg_to_object(jg_value value);

#endif
