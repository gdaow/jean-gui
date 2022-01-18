#include <string.h>
#include <minunit.h>

#include <uiligi/object.h>
#include <uiligi/template.h>

#include "test_object.h"
#include "fixtures/user.h"
#include "fixtures/admin.h"

MU_TEST(template_instanciate) {
    UlgContext* context = ulg_context_new();
    ulg_class_register(context, user_type);
    ulg_class_register(context, admin_type);

    UlgTemplate* template = ulg_template_new(context, "Admin");

    const char* name = "uber jean-mi";
    const char* role = "manager of all the jean-mi's";
    ulg_template_set_scalar(template, "name", name);
    ulg_template_set_scalar(template, "role", role);

    Admin* admin = (Admin*)ulg_template_instanciate(template);
    mu_check(!strcmp(admin->base.name, name));
    mu_check(!strcmp(admin->role, role));
    ulg_object_free((UlgObject*)admin);
    ulg_template_free(template);
    ulg_context_free(context);
}

MU_TEST_SUITE(template_suite) {
    MU_RUN_TEST(template_instanciate);
}
