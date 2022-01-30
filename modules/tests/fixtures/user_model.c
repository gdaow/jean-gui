/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <assert.h>
#include <stdalign.h>

#include <jgui/object.h>
#include <jgui/core.h>
#include <jgui/class.h>
#include <jgui/value.h>

#include "jgui/module.h"
#include "user_model.h"

const char* user_model_ns = "http://ki-dour.org/jean-gui/tests/user-model";
const char* user_class_id = "User";
const char* admin_class_id = "Admin";
const char* team_class_id = "Team";

void register_user_class(jg_module_definition* module);
void register_admin_class(jg_module_definition* module);
void register_team_class(jg_module_definition* module);

void user_model_plugin(jg_context_definition* context_definition) {
    jg_module_definition* module = jg_context_add_module(context_definition, user_model_ns);
    register_user_class(module);
    register_admin_class(module);
    register_team_class(module);
}

static jg_value user_get_name(const void* object) {
    const user* user = object;
    return jg_string(user->name);
}

static void user_set_name(void* object, jg_value value) {
    user* user = object;
    user->name = jg_to_string(value);
}

static jg_value user_get_team(const void* object) {
    const user* user = object;
    team* team = user->team;
    assert(team);
    return jg_object(user->team);
}

static void user_set_team(void* object, jg_value value) {
    user* user = object;
    user->team = jg_to_object(value);
}

static const char* user_has_permission_id = "has_permissions";

void user_constructor(void* object) {
    user* user = object;
    user->constructed_class_id = user_class_id;
}

void user_destructor(void* object) {
    user* user = object;
    if(user->destructed_class_id) {
        *(user->destructed_class_id) = user_class_id;
    }
}

bool user_has_permission(user* user, permission_flags flags) {
    return jg_to_bool(
        jg_object_call(user, user_has_permission_id, (jg_value[]) {
            jg_int(flags), jg_none()
        })
    );
}

static jg_value user_has_permission_impl(jg_value* arguments) {
    assert(jg_is_int(arguments[0]));
    assert(jg_is_none(arguments[1]));

    return jg_bool(jg_to_int(arguments[0]) & (PERM_LOGIN | PERM_CHANGE_PASSWORD));
}

void register_user_class(jg_module_definition* module) {
    jg_class_definition* class_ = jg_module_add_class(
        module,
        user_class_id,
        jg_core_ns,
        jg_object_class_id,
        sizeof(user)
    );
    jg_class_set_constructor(class_, user_constructor, user_destructor);
    jg_class_add_property(class_, "name", user_get_name, user_set_name);
    jg_class_add_property(class_, "team", user_get_team, user_set_team);
    jg_class_add_method(class_, user_has_permission_id, user_has_permission_impl);
}

static jg_value admin_get_role(const void* object) {
    const admin* admin = (void *)object;
    return jg_string(admin->role);
}

static void admin_set_role(void* object, jg_value value) {
    admin* admin = object;
    admin->role = jg_to_string(value);
}

static void admin_constructor(void* object) {
    admin* admin = object;
    admin->base.constructed_class_id = admin_class_id;
}

static void admin_destructor(void* object) {
    admin* admin = object;
    if(admin->base.destructed_class_id) {
        *(admin->base.destructed_class_id) = admin_class_id;
    }
}

static jg_value admin_has_permission(jg_value* arguments) {
    assert(jg_is_int(arguments[0]));
    assert(jg_is_none(arguments[1]));
    return jg_bool(jg_to_int(arguments[0]) & PERM_ALL);
}


void register_admin_class(jg_module_definition* module) {
    jg_class_definition* class_ = jg_module_add_class(
        module,
        admin_class_id,
        NULL,
        user_class_id,
        sizeof(admin)
    );
    jg_class_set_constructor(class_, admin_constructor, admin_destructor);
    jg_class_add_property(class_, "role", admin_get_role, admin_set_role);
    jg_class_add_method(class_, user_has_permission_id, admin_has_permission);
}

static void team_set_name(void* object, jg_value value) {
    team* team = object;
    team->name = jg_to_string(value);
}

static jg_value team_get_name(const void* object) {
    const team* team = object;
    return jg_string(team->name);
}

void register_team_class(jg_module_definition* module) {
    jg_class_definition* class_ = jg_module_add_class(
        module,
        team_class_id,
        jg_core_ns,
        jg_object_class_id,
        sizeof(team)
    );
    jg_class_add_property(class_, "name", team_get_name, team_set_name);
}
