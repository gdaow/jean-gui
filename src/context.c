#include <stdlib.h>
#include "wxligi/context.h"

struct WlgContext {
    int dummy;
};

WlgContext* wlg_context_create() {
    return malloc(sizeof(WlgContext));
}
