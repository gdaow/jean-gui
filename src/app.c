#include <stdlib.h>
#include "wxligi/app.h"

struct WlgApp {
    int dummy;
};

WlgApp* wlg_app_create() {
    return malloc(sizeof(WlgApp));
}