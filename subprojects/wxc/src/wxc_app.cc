#include "wxc_app.h"

bool WxcApp::OnInit() {
    return on_init(this);
}

WxcApp* wxc_app_create() {
    return new WxcApp();
}

void wxc_app_destroy(WxcApp* app) {
    delete app;
}

void wxc_app_on_init(WxcApp* app, WxcOnInitCallback callback, void* data) {
    app->on_init.set(callback, data);
}

bool wxc_app_on_init_parent(WxcApp* app) {
    return app->wxApp::OnInit();
}

static WxcApp* g_app;

wxAppConsole* _wxc_initialize() {
    return g_app;
}

void wxc_app_run(WxcApp* app) {
    g_app = app;
    wxApp::SetInitializerFunction(_wxc_initialize);
    int argc = 1;
    char** argv = new char*[1];
    argv[0] = new char[4];
    argv[0][1] = 'a';
    argv[0][2] = 0;
    wxEntry(argc, argv);
}
