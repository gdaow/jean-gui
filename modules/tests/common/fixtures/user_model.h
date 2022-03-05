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
#include <jgui/core/context.h>
#include <stdbool.h>

typedef struct admin_s admin;
typedef struct team_s team;
typedef struct user_s user;

typedef enum {
    PERM_LOGIN = 1,
    PERM_CHANGE_PASSWORD = 0x2,
    PERM_CREATE_USER = 0x4,
    PERM_DELETE_USER = 0x8,
    PERM_ALL = PERM_LOGIN | PERM_CHANGE_PASSWORD | PERM_CREATE_USER | PERM_DELETE_USER
} permission_flags;

struct user_s {
    const char* name;
    team* team;
};

struct admin_s {
    user base;
    const char* role;
};

struct team_s {
    const char* name;
};

