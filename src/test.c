#include "wxc/app.h"
#include <stdio.h>

bool on_init(WxcApp* app, void* data) {
    const char* test = (const char*)data;
    return wxc_app_on_init_parent(app);
}

int main(int argc, char** argv) {
    WxcApp* app = wxc_app_create();
    wxc_app_on_init(app, on_init, "Hello");
    wxc_app_run(app);
    wxc_app_destroy(app);
    return 0;
}
