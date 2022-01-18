#pragma once

typedef struct _UlgClass UlgClass;
typedef struct _UlgClassFactory UlgClassFactory;
typedef struct _Team Team;

typedef struct {
    const char* name;
    Team* team;
} User;

typedef struct {
    UlgObjectVT base;
    void (*set_default_name)(User*);
} UserVT;

const UlgClass* user(UlgClassFactory* factory);

void user_set_default_name(User* user);
