#include "wxc/app.h"
#include "wxligi.h"
#include <stdio.h>

bool on_init(WxcApp* app, void* data) {
    WlgApp* wlg_app = wlg_app_create();
    WlgTemplate* template = wlg_template_from_string(
        wlg_app,
        "frame:"
        "  box:"
        "    - button:"
        "        label: 'Hello World'"
    );

    wlg_template_instanciate(template);
    /*
    wlg_frame_show(frame, true);
    */
    return wxc_app_on_init_parent(app);
}

int main(int argc, char** argv) {
    WxcApp* app = wxc_app_create();
    wxc_app_on_init(app, on_init, "Hello");
    wxc_app_run(app);
    wxc_app_destroy(app);
    return 0;
}
