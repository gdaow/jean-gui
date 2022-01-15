#pragma once

/**
 * The WlgContext contains all informations needed to instanciate widgets from source files.
 */
typedef struct WlgContext WlgContext;


/**
 * Create a new context.
 */
WlgContext* wlg_context_create();
