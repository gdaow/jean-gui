#pragma once

struct UlgClass;

struct UlgObject {
    UlgClass* class_;
    char data[];
};
