#pragma once

#include <uiligi/object.h>

typedef struct _UlgClass UlgClass;
typedef struct _UlgClassFactory UlgClassFactory;


typedef struct User {
    const char* name;
} User;

typedef struct {
    UlgObjectVT base;
    void (*set_default_name)(User*);
} UserVT;

const UlgClass* user(UlgClassFactory* factory);

void user_set_default_name(User* user);
