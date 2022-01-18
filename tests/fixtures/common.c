#include "common.h"

#include <uiligi/object.h>

#include "admin.h"
#include "user.h"
#include "team.h"

UlgContext* fixture_context_new() {
    UlgContext* context = ulg_context_new();
    ulg_class_register(context, user_type);
    ulg_class_register(context, admin_type);
    ulg_class_register(context, team_type);
    return context;
}