#pragma once
#include "wxligi/app.h"

typedef struct WlgTemplate WlgTemplate;

/**
 * Create a template from a YAML source string.
 *
 * @param app Previously created and initialized WlgApp.
 */
WlgTemplate* wlg_template_from_string(WlgApp* app, const char* source);

/**
 * Instanciate a template to a widget hierarchy and, binds the given view model to it.
 */
void wlg_template_instanciate(WlgTemplate* template);
