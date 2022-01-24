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

void register_user_type(UlgModuleDefinition* module);
void register_admin_type(UlgModuleDefinition* module);
void register_team_type(UlgModuleDefinition* module);

UlgModule* user_model_module_new() {
    UlgModuleDefinition* module = ulg_module_new();
    register_user_type(module);
    register_admin_type(module);
    register_team_type(module);
    return ulg_module_build(module);
}
static UlgValue _user_get_name(const void* object) {
    const User* user = object;
    return ulg_string(user->name);
}

static void _user_set_name(void* object, UlgValue value) {
    User* user = object;
    user->name = ulg_to_string(value);
}

static UlgValue _user_get_team(const void* object) {
    const User* user = object;
    Team* team = user->team;
    assert(team);
    return ulg_object(user->team);
}

static void _user_set_team(void* object, UlgValue value) {
    User* user = object;
    user->team = ulg_to_object(value);
}

void register_user_type(UlgModuleDefinition* module) {
    UlgClassDefinition* class_ = ulg_class_new(module, USER, ULG_OBJECT, sizeof(User), alignof(User));
    ulg_class_add_property(class_, "name", _user_get_name, _user_set_name);
    ulg_class_add_property(class_, "team", _user_get_team, _user_set_team);
}

static UlgValue _admin_get_role(const void* object) {
    const Admin* admin = (void *)object;
    return ulg_string(admin->role);
}

static void _admin_set_role(void* object, UlgValue value) {
    Admin* admin = object;
    admin->role = ulg_to_string(value);
}

void register_admin_type(UlgModuleDefinition* module) {
    UlgClassDefinition* class_ = ulg_class_new(module, ADMIN, USER, sizeof(Admin), alignof(Admin));
    ulg_class_add_property(class_, "role", _admin_get_role, _admin_set_role);
}

static void _team_set_name(void* object, UlgValue value) {
    Team* team = object;
    team->name = ulg_to_string(value);
}

static UlgValue _team_get_name(const void* object) {
    const Team* team = object;
    return ulg_string(team->name);
}

void register_team_type(UlgModuleDefinition* module) {
    UlgClassDefinition* class_ = ulg_class_new(module, TEAM, ULG_OBJECT, sizeof(Team), alignof(Team));
    ulg_class_add_property(class_, "name", _team_get_name, _team_set_name);
}
