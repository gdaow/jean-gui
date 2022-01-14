#include "wxc/app.h"

int main(int argc, char** argv) {
    WxcApp* app = wxc_app_create();
    wxc_app_destroy(app);
    return 0;
}
