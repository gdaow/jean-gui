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

UlgModule* user_model_module_new() {
    UlgModule* module = ulg_module_new();
    ulg_class_register(module, user_type);
    ulg_class_register(module, admin_type);
    ulg_class_register(module, team_type);
    return module;
}

static PermissionFlags _user_get_default_permissions() {
    return PERM_CAN_LOGIN;
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

typedef struct {
    UlgObjectVT base;
    PermissionFlags (*get_default_permissions)();
} UserVT;

void user_type(UlgClassFactory* factory) {
    ulg_class_declare(factory, "User", ulg_object_type);

    ulg_class_add_property(factory, "name", _user_get_name, _user_set_name);
    ulg_class_add_property(factory, "team", _user_get_team, _user_set_team);

    UserVT* vtable = ulg_class_create_vtable(factory, sizeof(UserVT), alignof(UserVT));
    vtable->get_default_permissions = _user_get_default_permissions;
}

PermissionFlags user_get_default_permissions(User* user) {
    return ((const UserVT*)ulg_object_vtable((void*)user))->get_default_permissions();
}

static PermissionFlags _admin_get_default_permissions() {
    return PERM_ALL;
}

static UlgValue _admin_get_role(const void* object) {
    const Admin* admin = (void *)object;
    return ulg_string(admin->role);
}

static void _admin_set_role(void* object, UlgValue value) {
    Admin* admin = object;
    admin->role = ulg_to_string(value);
}

void admin_type(UlgClassFactory* factory) {
    ulg_class_declare(factory, "Admin", user_type);
    ulg_class_add_property(factory, "role", _admin_get_role, _admin_set_role);

    UserVT* vtable = ulg_class_create_vtable(factory, sizeof(UserVT), alignof(UserVT));
    vtable->get_default_permissions = _admin_get_default_permissions;
}

static void _team_set_name(void* object, UlgValue value) {
    Team* team = object;
    team->name = ulg_to_string(value);
}

static UlgValue _team_get_name(const void* object) {
    const Team* team = object;
    return ulg_string(team->name);
}

void team_type(UlgClassFactory* factory) {
    ulg_class_declare(factory, "Team", ulg_object_type);
    ulg_class_add_property(factory, "name", _team_get_name, _team_set_name);
}
