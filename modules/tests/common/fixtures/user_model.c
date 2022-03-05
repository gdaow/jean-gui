/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <assert.h>
#include <stdalign.h>

#include <jgui/object-model/object.h>
#include <jgui/core/core.h>
#include <jgui/core/context.h>
#include <jgui/object-model/class.h>
#include <jgui/object-model/value.h>

#include "user_model.h"

/*C
static void register_user_class(jg_context* context);
static void register_admin_class(jg_context* context);
static void register_team_class(jg_context* context);

jg_id user_model_class_id(const char* name) {
    return jg_id_new("http://ki-dour.org/jean-gui/tests/user-model", name);
}

jg_id user_class_id() { return user_model_class_id("User"); }
jg_id admin_class_id() { return user_model_class_id("Admin"); }
jg_id team_class_id() { return user_model_class_id("Team"); }

void user_model_plugin(jg_context* context) {
    register_user_class(context);
    register_admin_class(context);
    register_team_class(context);
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
    user->constructed_class_id = user_class_id();
}

void user_destructor(void* object) {
    user* user = object;
    if(user->destructed_class_id) {
        *(user->destructed_class_id) = user_class_id();
    }
}

bool user_has_permission(user* user, permission_flags flags) {
    return jg_to_bool(
        jg_object_call(user, user_has_permission_id, (jg_value[]) {
            jg_int(flags), jg_none()
        })
    );
}

static jg_value user_has_permission_impl(jg_arguments* args) {
    (void)args;
    /*
    int flags = jg_pop_int(args);

    if(jg_arguments_error(args)) {
        return jg_none();
    }

    return jg_bool(flags & (PERM_LOGIN | PERM_CHANGE_PASSWORD));
    */
    return jg_none();
}

void register_user_class(jg_context* context) {
    jg_class* class_ = jg_context_add_class(
        context,
        user_class_id(),
        jg_object_class_id(),
        sizeof(user)
    );
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

/*
static void admin_constructor(void* object) {
    admin* admin = object;
    admin->base.constructed_class_id = admin_class_id();
}

static void admin_destructor(void* object) {
    admin* admin = object;
    if(admin->base.destructed_class_id) {
        *(admin->base.destructed_class_id) = admin_class_id();
    }
}

static jg_value admin_has_permission(jg_arguments* args) {
    (void)args;
    int flags = jg_pop_int(args);
    if(jg_arguments_error(args)) {
        return jg_none();
    }

    return jg_bool(flags & PERM_ALL);
    return jg_none();
}


void register_admin_class(jg_context* context) {
    jg_class* class_ = jg_context_add_class(
        context,
        admin_class_id(),
        user_class_id(),
        sizeof(admin)
    );
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

void register_team_class(jg_context* context) {
    jg_class* class_ = jg_context_add_class(
        context,
        team_class_id(),
        jg_object_class_id(),
        sizeof(team)
    );
    jg_class_add_property(class_, "name", team_get_name, team_set_name);
}

*/

