/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <assert.h>
#include <stdalign.h>

#include <uiligi/object.h>

#include "user_model.h"

void register_user_type(ulg_module_definition_t* module);
void register_admin_type(ulg_module_definition_t* module);
void register_team_type(ulg_module_definition_t* module);

ulg_module_t* user_model_module_new() {
    ulg_module_definition_t* module = ulg_module_new();
    register_user_type(module);
    register_admin_type(module);
    register_team_type(module);
    return ulg_module_build(module);
}
static ulg_value_t user_get_name(const void* object) {
    const user_t* user = object;
    return ulg_string(user->name);
}

static void user_set_name(void* object, ulg_value_t value) {
    user_t* user = object;
    user->name = ulg_to_string(value);
}

static ulg_value_t user_get_team(const void* object) {
    const user_t* user = object;
    team_t* team = user->team;
    assert(team);
    return ulg_object(user->team);
}

static void user_set_team(void* object, ulg_value_t value) {
    user_t* user = object;
    user->team = ulg_to_object(value);
}

void register_user_type(ulg_module_definition_t* module) {
    ulg_class_definition_t* class_ = ulg_class_new(module, USER, ULG_OBJECT, sizeof(user_t), alignof(user_t));
    ulg_class_add_property(class_, "name", user_get_name, user_set_name);
    ulg_class_add_property(class_, "team", user_get_team, user_set_team);
}

static ulg_value_t admin_get_role(const void* object) {
    const admin_t* admin = (void *)object;
    return ulg_string(admin->role);
}

static void admin_set_role(void* object, ulg_value_t value) {
    admin_t* admin = object;
    admin->role = ulg_to_string(value);
}

void register_admin_type(ulg_module_definition_t* module) {
    ulg_class_definition_t* class_ = ulg_class_new(module, ADMIN, USER, sizeof(admin_t), alignof(admin_t));
    ulg_class_add_property(class_, "role", admin_get_role, admin_set_role);
}

static void team_set_name(void* object, ulg_value_t value) {
    team_t* team = object;
    team->name = ulg_to_string(value);
}

static ulg_value_t team_get_name(const void* object) {
    const team_t* team = object;
    return ulg_string(team->name);
}

void register_team_type(ulg_module_definition_t* module) {
    ulg_class_definition_t* class_ = ulg_class_new(module, TEAM, ULG_OBJECT, sizeof(team_t), alignof(team_t));
    ulg_class_add_property(class_, "name", team_get_name, team_set_name);
}
