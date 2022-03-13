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
#include <jgui/misc/utils.h>

struct test_team_s;

jg_forward_declare(test_team);

typedef struct test_user_s {
	const char* name;
	test_team* team;
} test_user;

typedef struct test_admin_s {
	test_user base;
	const char* role;
} test_admin;

typedef struct test_team_s {
	const char* name;
} test_team;

const char* user_get_name(const test_user* user);

void user_set_name(test_user* user, const char* name);
