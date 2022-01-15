#include "wxc/app.h"
#include "uiligi.h"
#include <stdio.h>

bool on_init(WxcApp* app, void* data) {
    UlgContext* context = ulg_context_create();
    UlgTemplate* template = ulg_template_from_string(
        context,
        "frame:"
        "  box:"
        "    - button:"
        "        label: 'Hello World'"
    );

    ulg_template_instanciate(template);
    /*
    ulg_frame_show(frame, true);
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
