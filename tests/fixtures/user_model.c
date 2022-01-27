/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <assert.h>
#include <stdalign.h>

#include <jg/object.h>

#include "user_model.h"

void register_user_type(jg_module_definition* module);
void register_admin_type(jg_module_definition* module);
void register_team_type(jg_module_definition* module);

jg_module* user_model_module_new() {
    jg_module_definition* module = jg_module_new();
    register_user_type(module);
    register_admin_type(module);
    register_team_type(module);
    return jg_module_build(module);
}
static jg_value user_get_name(const void* object) {
    const user_t* user = object;
    return jg_string(user->name);
}

static void user_set_name(void* object, jg_value value) {
    user_t* user = object;
    user->name = jg_to_string(value);
}

static jg_value user_get_team(const void* object) {
    const user_t* user = object;
    team_t* team = user->team;
    assert(team);
    return jg_object(user->team);
}

static void user_set_team(void* object, jg_value value) {
    user_t* user = object;
    user->team = jg_to_object(value);
}

void register_user_type(jg_module_definition* module) {
    jg_class_definition* class_ = jg_class_new(module, USER, jg_OBJECT, sizeof(user_t), alignof(user_t));
    jg_class_add_property(class_, "name", user_get_name, user_set_name);
    jg_class_add_property(class_, "team", user_get_team, user_set_team);
}

static jg_value admin_get_role(const void* object) {
    const admin_t* admin = (void *)object;
    return jg_string(admin->role);
}

static void admin_set_role(void* object, jg_value value) {
    admin_t* admin = object;
    admin->role = jg_to_string(value);
}

void register_admin_type(jg_module_definition* module) {
    jg_class_definition* class_ = jg_class_new(module, ADMIN, USER, sizeof(admin_t), alignof(admin_t));
    jg_class_add_property(class_, "role", admin_get_role, admin_set_role);
}

static void team_set_name(void* object, jg_value value) {
    team_t* team = object;
    team->name = jg_to_string(value);
}

static jg_value team_get_name(const void* object) {
    const team_t* team = object;
    return jg_string(team->name);
}

void register_team_type(jg_module_definition* module) {
    jg_class_definition* class_ = jg_class_new(module, TEAM, jg_OBJECT, sizeof(team_t), alignof(team_t));
    jg_class_add_property(class_, "name", team_get_name, team_set_name);
}
