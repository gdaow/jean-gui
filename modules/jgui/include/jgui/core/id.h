/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * jg_id is a generic identifier, consisting of a namespace and a name.
 * It's used to avoid name collisions for objects named with a string in
 * Jean-Gui (like classes).
 *
 */
#ifndef JGUI_CORE_ID
#define JGUI_CORE_ID

#include <stdbool.h>

/**
  * /!\ Private struct (see doc-private-struct) /!\
  */
typedef struct jg_id_s {
    const char* namespace_;
    const char* name;
} jg_id;

/** NULL identifier constant */
extern const jg_id JG_ID_NONE;

/*
 * Create a new identifier.
 *
 * @param namespace_ The identifier namespace.
 * @param name       The identifier name.
 *
 * @return A new id for the given namespace and name.
 */
jg_id JG_ID(const char* namespace_, const char* name);

/*
 * Check if an identifier is the NULL identifier.
 *
 * @param id The identifier to check.
 *
 * @return True if the given identifier is the NULL identifier, true otherwise.
 */
bool jg_id_is_none(jg_id id);

/*
 * Check if an identifier is correctly constructed.
 * That is, either it's the NULL identifier, either both namespace and name are
 * defined and not empty.
 *
 * @param id The identifier to check.
 *
 * @return True if the given identifier correctly constructed, false otherwise.
 */
bool jg_id_is_valid(jg_id id);

#endif

