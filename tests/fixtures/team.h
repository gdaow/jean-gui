#pragma once

typedef struct _UlgClass UlgClass;
typedef struct _UlgClassFactory UlgClassFactory;

typedef struct _Team {
    const char* name;
} Team;


const UlgClass* team_type(UlgClassFactory* factory);

void team_set_default_name(Team* team);
