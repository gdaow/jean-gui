#pragma once

#include "uiligi/class.h"

struct UlgClass {
    const char* name;
    UlgConstructor constructor;
    UlgDestructor destructor;
};
