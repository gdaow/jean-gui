#pragma once

#include "uiligi/context.h"

typedef struct UlgTemplate UlgTemplate;

/**
 * Create a template from a YAML source string.
 *
 * @param context Previously created and initialized UlgContext.
 */
UlgTemplate* ulg_template_from_string(UlgContext* context, const unsigned char* source);

/**
 * Release the given template
 */
void ulg_template_destroy(UlgTemplate* template);

/**
 * Instanciate a template to a widget hierarchy and, binds the given view model to it.
 */
void ulg_template_instanciate(UlgTemplate* template);
