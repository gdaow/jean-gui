#pragma once

typedef struct _UlgClass UlgClass;
typedef struct _UlgClassFactory UlgClassFactory;

typedef struct User {
    const char* name;
} User;

const UlgClass* user(UlgClassFactory* factory);
