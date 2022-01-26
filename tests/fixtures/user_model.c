/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <assert.h>
#include <stdalign.h>

#include <mezo/object.h>

#include "user_model.h"

void register_user_type(mz_module_definition* module);
void register_admin_type(mz_module_definition* module);
void register_team_type(mz_module_definition* module);

mz_module* user_model_module_new() {
    mz_module_definition* module = mz_module_new();
    register_user_type(module);
    register_admin_type(module);
    register_team_type(module);
    return mz_module_build(module);
}
static mz_value user_get_name(const void* object) {
    const user_t* user = object;
    return mz_string(user->name);
}

static void user_set_name(void* object, mz_value value) {
    user_t* user = object;
    user->name = mz_to_string(value);
}

static mz_value user_get_team(const void* object) {
    const user_t* user = object;
    team_t* team = user->team;
    assert(team);
    return mz_object(user->team);
}

static void user_set_team(void* object, mz_value value) {
    user_t* user = object;
    user->team = mz_to_object(value);
}

void register_user_type(mz_module_definition* module) {
    mz_class_definition* class_ = mz_class_new(module, USER, mz_OBJECT, sizeof(user_t), alignof(user_t));
    mz_class_add_property(class_, "name", user_get_name, user_set_name);
    mz_class_add_property(class_, "team", user_get_team, user_set_team);
}

static mz_value admin_get_role(const void* object) {
    const admin_t* admin = (void *)object;
    return mz_string(admin->role);
}

static void admin_set_role(void* object, mz_value value) {
    admin_t* admin = object;
    admin->role = mz_to_string(value);
}

void register_admin_type(mz_module_definition* module) {
    mz_class_definition* class_ = mz_class_new(module, ADMIN, USER, sizeof(admin_t), alignof(admin_t));
    mz_class_add_property(class_, "role", admin_get_role, admin_set_role);
}

static void team_set_name(void* object, mz_value value) {
    team_t* team = object;
    team->name = mz_to_string(value);
}

static mz_value team_get_name(const void* object) {
    const team_t* team = object;
    return mz_string(team->name);
}

void register_team_type(mz_module_definition* module) {
    mz_class_definition* class_ = mz_class_new(module, TEAM, mz_OBJECT, sizeof(team_t), alignof(team_t));
    mz_class_add_property(class_, "name", team_get_name, team_set_name);
}
