/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 * TODO:
 *  - document.
 *
 */
#ifndef JGUI_PRIVATE_MISC_ASSERT_H
#define JGUI_PRIVATE_MISC_ASSERT_H
#include <stdbool.h>

#include "jgui/static-config.h"


#if __clang__
#endif

#if JG_ENABLE_ASSERTS
#	if __clang__
#		pragma clang diagnostic push
#		pragma clang diagnostic ignored "-Wunused-function"

			static void shut_up_clang_tidy() __attribute__((analyzer_noreturn)) {}

#		pragma clang diagnostic pop
#		define assert(TEST) {(jg_assert_impl(TEST, #    TEST, __FILE__, __LINE__)); if (!(TEST)) shut_up_clang_tidy(); }
#else
#		define assert(TEST) {(jg_assert_impl(TEST, #    TEST, __FILE__, __LINE__)); }
#endif

void jg_assert_impl(bool test_result,
                    const char* test_expression,
                    const char* file,
                    int line);

#else
#	define assert(TEST)
#endif

#endif
