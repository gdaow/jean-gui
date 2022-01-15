#pragma once

typedef enum {
    ULG_STRING
} UlgValueType;

typedef struct UlgValue {
    UlgValueType type;
    void* data;
} UlgValue;

/**
 * Constuct a generic value containing a string.
 * @param value The string value to assign to this value container.
 * @return The created value.
 */
UlgValue ulg_value_from_str(const char* value);

/**
 * Get a string form a generic value. Will assert if the given value is of the wrong type.
 * @param value The generic value to convert to string.
 * @return The string contained in the generic value.
 */
const char* ulg_value_to_str(UlgValue value);
