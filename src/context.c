#include <stdlib.h>
#include "uiligi/context.h"

struct UlgContext {
    int dummy;
};

UlgContext* ulg_context_create() {
    return malloc(sizeof(UlgContext));
}
