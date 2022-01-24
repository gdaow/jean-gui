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
typedef struct _UlgClassDefinition UlgClassDefinition;

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

struct _User {
    const char* name;
    Team* team;
};

struct _Admin {
    User base;
    const char* role;
};

struct _Team {
    const char* name;
};


#define USER "User"
#define ADMIN "Admin"
#define TEAM "Team"

UlgModule* user_model_module_new();
PermissionFlags user_get_default_permissions(User* user);
