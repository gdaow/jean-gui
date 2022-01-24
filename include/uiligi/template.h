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
 #ifndef INCLUDE_UILIGI_TEMPLATE_H
 #define INCLUDE_UILIGI_TEMPLATE_H

typedef struct ulg_class_s ulg_class_t;
typedef struct ulg_module_s ulg_module_t;
typedef struct ulg_template_s ulg_template_t;

/**
 * @brief Create a new template_ node.
 * 
 * @return ulg_template_t* 
 */
ulg_template_t* ulg_template_from_string(const char* source, ulg_module_t* module);

/**
 * @brief Destroy the given template_ node, and all of it's children.
 * 
 * @param template_ The template_ to destroy.
 */
void ulg_template_free(ulg_template_t* template_);

/**
 * @brief Create an object based on the specified template_.
 * 
 * @param template_ The template_ to instanciate.
 * @return The created object.
 */
void* ulg_template_instanciate(const ulg_template_t* template_);

#endif
