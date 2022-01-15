#include "wxc/app.h"
#include <stdio.h>

struct TestViewModelData {
    char* text_value;
}

void set_text_value(WlgViewModel* view_model, char* value) {
    TestViewModelData* data = (TestViewModelData*)wlg_view_model_get_data(view_model);
    data->text_value = value;
}

char* get_text_value(WlgViewModel* view_model) {
    TestViewModelData* data = (TestViewModelData*)wlg_view_model_get_data(view_model);
    return data->text_value;
}

bool on_init(WxcApp* app, void* data) {
    Wlg* wlg = wlg_init();
    WlgTemplate* template = wlg_template_from_string(
        wlg,
        "frame:"
        "  box:"
        "    - button:"
        "        label: 'Hello World'"
    );

    WlgFrame* frame = wlg_template_instanciate(template, NULL);
    wlg_frame_show(frame, true);
    return wxc_app_on_init_parent(app);
}

int main(int argc, char** argv) {
    WxcApp* app = wxc_app_create();
    wxc_app_on_init(app, on_init, "Hello");
    wxc_app_run(app);
    wxc_app_destroy(app);
    return 0;
}
