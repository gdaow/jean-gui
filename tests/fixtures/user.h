#pragma once

typedef struct _UlgClass UlgClass;
typedef struct _UlgClassFactory UlgClassFactory;
typedef struct _Team Team;

typedef enum {
    PERM_CAN_LOGIN = 1,
    PERM_CAN_CHANGE_PASSWORD = 0x2,
    PERM_CAN_CREATE_USER = 0x4,
    PERM_CAN_DELETE_USER = 0x8,
    PERM_ALL = PERM_CAN_LOGIN | PERM_CAN_CHANGE_PASSWORD | PERM_CAN_CREATE_USER | PERM_CAN_DELETE_USER
} PermissionFlags;

typedef struct {
    const char* name;
    Team* team;
} User;

typedef struct {
    UlgObjectVT base;
    PermissionFlags (*get_default_permissions)();
} UserVT;

const UlgClass* user_type(UlgClassFactory* factory);

PermissionFlags user_get_default_permissions(User* user);
