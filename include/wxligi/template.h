#pragma once
#include "wxligi/context.h"

typedef struct WlgTemplate WlgTemplate;

/**
 * Create a template from a YAML source string.
 *
 * @param context Previously created and initialized WlgContext.
 */
WlgTemplate* wlg_template_from_string(WlgContext* context, const char* source);

/**
 * Instanciate a template to a widget hierarchy and, binds the given view model to it.
 */
void wlg_template_instanciate(WlgTemplate* template);
