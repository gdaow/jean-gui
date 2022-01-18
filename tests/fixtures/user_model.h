/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 * A dummy user data model, used to test ulg object model & templating.
 *
 */
#pragma once

typedef struct _UlgClass UlgClass;
typedef struct _UlgClassFactory UlgClassFactory;

typedef struct _Admin Admin;
typedef struct _Team Team;
typedef struct _User User;

typedef enum {
    PERM_CAN_LOGIN = 1,
    PERM_CAN_CHANGE_PASSWORD = 0x2,
    PERM_CAN_CREATE_USER = 0x4,
    PERM_CAN_DELETE_USER = 0x8,
    PERM_ALL = PERM_CAN_LOGIN | PERM_CAN_CHANGE_PASSWORD | PERM_CAN_CREATE_USER | PERM_CAN_DELETE_USER
} PermissionFlags;

UlgContext* user_model_context_new();

const UlgClass* user_type(UlgClassFactory* factory);

const char* user_get_name(const User* user);
void user_set_name(User* user, const char* name);

PermissionFlags user_get_default_permissions(User* user);

const UlgClass* admin_type(UlgClassFactory* factory);

const char* admin_get_role(const Admin* admin);
void admin_set_role(Admin* admin, const char* role);

const UlgClass* team_type(UlgClassFactory* factory);
