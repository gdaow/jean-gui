/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * Templating allows to create a schema for a hierarchy of objects.
 *
 * TODO: Finish documentation here.
 *
 */
 #ifndef INCLUDE_JEAN_GUI_TEMPLATE_H
 #define INCLUDE_JEAN_GUI_TEMPLATE_H

typedef struct jg_class_s jg_class;
typedef struct jg_module_s jg_module;
typedef struct jg_template_s jg_template;

/**
 * @brief Create a new template_ node.
 * 
 * @return jg_template* 
 */
jg_template* jg_template_from_string(const char* source, jg_module* module);

/**
 * @brief Destroy the given template_ node, and all of it's children.
 * 
 * @param template_ The template_ to destroy.
 */
void jg_template_free(jg_template* template_);

/**
 * @brief Create an object based on the specified template_.
 * 
 * @param template_ The template_ to instanciate.
 * @return The created object.
 */
void* jg_template_instanciate(const jg_template* template_);

#endif
