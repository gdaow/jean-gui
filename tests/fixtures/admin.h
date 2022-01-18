#pragma once

#include "user.h"

typedef struct _UlgClass UlgClass;
typedef struct _UlgClassFactory UlgClassFactory;

typedef struct {
    User base;
    const char* role;
} Admin;

const UlgClass* admin_type(UlgClassFactory* factory);
