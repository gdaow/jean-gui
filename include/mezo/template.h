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
 #ifndef INCLUDE_MEZO_TEMPLATE_H
 #define INCLUDE_MEZO_TEMPLATE_H

typedef struct mz_class_s mz_class;
typedef struct mz_module_s mz_module;
typedef struct mz_template_s mz_template;

/**
 * @brief Create a new template_ node.
 * 
 * @return mz_template* 
 */
mz_template* mz_template_from_string(const char* source, mz_module* module);

/**
 * @brief Destroy the given template_ node, and all of it's children.
 * 
 * @param template_ The template_ to destroy.
 */
void mz_template_free(mz_template* template_);

/**
 * @brief Create an object based on the specified template_.
 * 
 * @param template_ The template_ to instanciate.
 * @return The created object.
 */
void* mz_template_instanciate(const mz_template* template_);

#endif
