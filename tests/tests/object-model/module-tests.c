/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "jgui/private/object-model/module.h"

#include <stddef.h>

#include "jgui/private/misc/assert.h"
#include "jgui/private/object-model/class.h"

#include "common/cmocka.h"

/** Classe should correctly be registered in modules. */
static void test_module_add_class(void** state) {
	(void)state;
	jg_module module;
	jg_module_init(&module);

	jg_class* class_ =
	    jg_module_add_class(&module, "test_class", NULL, sizeof(char));
	assert_non_null(class_);

	const jg_class* retrieved_class =
	    jg_module_get_class(&module, "test_class");

	assert(retrieved_class == class_);

	const jg_class* undeclared_class =
	    jg_module_get_class(&module, "undeclared_class");
	assert(undeclared_class == NULL);

	jg_module_cleanup(&module);
}

jg_begin_tests(NULL, NULL)
	cmocka_unit_test(test_module_add_class),
jg_end_tests()
