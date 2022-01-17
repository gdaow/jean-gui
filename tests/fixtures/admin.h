#pragma once

typedef struct _UlgClass UlgClass;
typedef struct _UlgClassFactory UlgClassFactory;

typedef struct {
    const char* role;
} Admin;

const UlgClass* admin(UlgClassFactory* factory);
