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

typedef struct _UlgClass UlgClass;
typedef struct _UlgModule UlgModule;
typedef struct _UlgTemplate UlgTemplate;

/**
 * @brief Create a new template node.
 * 
 * @return UlgTemplate* 
 */
UlgTemplate* ulg_template_new(UlgModule* module, const UlgClass* class_name);

/**
 * Return a new template node, with the class registered under the given name.
 * 
 * @param module 
 * @param class_name 
 * @return UlgTemplate* 
 */
UlgTemplate* ulg_template_new_by_name(UlgModule* module, const char* class_name);

/**
 * @brief Destroy the given template node, and all of it's children.
 * 
 * @param template The template to destroy.
 */
void ulg_template_free(UlgTemplate* template);

/**
 * @brief Add a scalar property to this template node.
 * 
 * @param template The template to add a property to.
 * @param name     The name of the property to assign this value to.
 * @param value    The value of the property.
 */
void ulg_template_set_scalar(UlgTemplate* template, const char* property_name, const char* value);

/**
 * @brief Adds a child object as a property.
 * 
 * @param template The template to add a property to.
 * @param name     The name of the property to assign the object to.
 * @param class    Class of the child object to create.
 */
UlgTemplate* ulg_template_set_child(UlgTemplate* template, const char* property_name, const char* class_name);

/**
 * @brief Create an object based on the specified template.
 * 
 * @param template The template to instanciate.
 * @return The created object.
 */
void* ulg_template_instanciate(const UlgTemplate* template);
