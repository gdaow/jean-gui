/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

void run_context_suite();
void run_object_suite();
void run_template_suite();
void run_value_suite();

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    run_context_suite();
    run_object_suite();
    run_template_suite();
    run_value_suite();
}
