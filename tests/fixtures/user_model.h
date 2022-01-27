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

typedef struct jg_module_s jg_module;

typedef struct admin_s admin_t;
typedef struct team_s team_t;
typedef struct user_s user_t;

typedef enum {
    PERM_CAN_LOGIN = 1,
    PERM_CAN_CHANGE_PASSWORD = 0x2,
    PERM_CAN_CREATE_USER = 0x4,
    PERM_CAN_DELETE_USER = 0x8,
    PERM_ALL = PERM_CAN_LOGIN | PERM_CAN_CHANGE_PASSWORD | PERM_CAN_CREATE_USER | PERM_CAN_DELETE_USER
} permission_flags_t;

struct user_s {
    const char* name;
    team_t* team;
};

struct admin_s {
    user_t base;
    const char* role;
};

struct team_s {
    const char* name;
};


#define USER "User"
#define ADMIN "Admin"
#define TEAM "Team"

jg_module* user_model_module_new();
permission_flags_t user_get_default_permissions(user_t* user);
