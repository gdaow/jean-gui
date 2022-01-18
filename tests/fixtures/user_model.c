/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <uiligi/object.h>

#include "user_model.h"

UlgContext* user_model_context_new() {
    UlgContext* context = ulg_context_new();
    ulg_class_register(context, user_type);
    ulg_class_register(context, admin_type);
    ulg_class_register(context, team_type);
    return context;
}

struct _User {
    const char* name;
    Team* team;
};

static PermissionFlags _user_get_default_permissions() {
    return PERM_CAN_LOGIN;
}

static UlgValue _user_get_name(const UlgObject* object) {
    const User* user = (void *)object;
    return ulg_value_from_str(user->name);
}

static void _user_set_name(UlgObject* object, UlgValue value) {
    User* user = (void *)object;
    user->name = ulg_value_to_str(value);
}

static UlgValue _user_get_team(const UlgObject* object) {
    const User* user = (void *)object;
    // TODO : Implement this.
    return ulg_value_from_str(user->name);
}

static void _user_set_team(UlgObject* object, UlgValue value) {
    User* user = (void *)object;
    user->name = ulg_value_to_str(value);
}

typedef struct {
    UlgObjectVT base;
    PermissionFlags (*get_default_permissions)();
} UserVT;

const UlgClass* user_type(UlgClassFactory* factory) {
    UlgClass* class_ = ulg_class_declare(
        factory,
        "User",
        sizeof(User),
        ulg_object_type,
        sizeof(UserVT)
    );

    ulg_class_add_property(class_, "name", _user_get_name, _user_set_name);
    ulg_class_add_property(class_, "team", _user_get_team, _user_set_team);

    UserVT* vtable = ulg_class_edit_vtable(class_);
    vtable->get_default_permissions = _user_get_default_permissions;
    return class_;
}

PermissionFlags user_get_default_permissions(User* user) {
    return ((const UserVT*)ulg_object_vtable((UlgObject*)user))->get_default_permissions();
}

const char* user_get_name(const User* user) {
    return user->name;
}

void user_set_name(User* user, const char* name) {
    user->name = name;
}

struct _Admin {
    User base;
    const char* role;
};

static PermissionFlags _admin_get_default_permissions() {
    return PERM_ALL;
}

static UlgValue _admin_get_role(const UlgObject* object) {
    const Admin* admin = (void *)object;
    return ulg_value_from_str(admin->role);
}

static void _admin_set_role(UlgObject* object, UlgValue value) {
    Admin* admin = (void *)object;
    admin->role = ulg_value_to_str(value);
}

const UlgClass* admin_type(UlgClassFactory* factory) {
    UlgClass* class_ = ulg_class_declare(
        factory,
        "Admin",
        sizeof(Admin),
        user_type,
        0
    );

    ulg_class_add_property(class_, "role", _admin_get_role, _admin_set_role);
    UserVT* vtable = ulg_class_edit_vtable(class_);
    vtable->get_default_permissions = _admin_get_default_permissions;

    return class_;
}

const char* admin_get_role(const Admin* admin) {
    return admin->role;
}

void admin_set_role(Admin* admin, const char* role) {
    admin->role = role;
}

struct _Team {
    const char* name;
};

static void _team_set_name(UlgObject* object, UlgValue value) {
    Team* team = (void *)object;
    team->name = ulg_value_to_str(value);
}

static UlgValue _team_get_name(const UlgObject* object) {
    const Team* team = (void *)object;
    return ulg_value_from_str(team->name);
}

const UlgClass* team_type(UlgClassFactory* factory) {
    UlgClass* class_ = ulg_class_declare(
        factory,
        "Team",
        sizeof(Team),
        ulg_object_type,
        0
    );

    ulg_class_add_property(class_, "name", _team_get_name, _team_set_name);

    return class_;
}
