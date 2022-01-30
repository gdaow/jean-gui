/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <assert.h>
#include <minunit.h>
#include <stdlib.h>

#include <jgui/context.h>

static char* last_error_message = NULL;
static jg_error_code last_error_code;

static void error_handler(jg_error_code error_code, const char* message) {
    if(last_error_message != NULL) {
        free(last_error_message);
    }
    size_t message_length = strlen(message);

    last_error_message = malloc(sizeof(char) * ( message_length + 1));
    strcpy(last_error_message, message);
    last_error_code = error_code;
}

void error_handler_plugin(jg_context_builder* context_builder) {
    jg_set_error_handler(context_builder, error_handler);
}


MU_TEST(test_error_handler) {
    assert(last_error_message == NULL);
    assert(last_error_code == JG_ERROR_NONE);

    jg_context* context = jg_context_load((jg_plugin[]) { error_handler_plugin, NULL });
    jg_error(context, JG_ERROR_CLASS_UNDEFINED, "Jean-GUI drinks a %s.", "coffee");
    assert(strcmp(last_error_message, "Jean-GUI drinks a coffee.") == 0);
    assert(last_error_code == JG_ERROR_CLASS_UNDEFINED);

    jg_context_free(context);
}

MU_TEST_SUITE(context_suite) {
    MU_RUN_TEST(test_error_handler);
}

