#include "wxc_app.h"

bool WxcApp::OnInit() {
    if(_on_init) {
        return (*_on_init)(_on_init_arg);
    }
    return wxApp::OnInit();
}

WxcApp* wxc_app_create() {
    return new WxcApp();
}

void wxc_app_destroy(WxcApp* app) {
    delete app;
}

void wxc_app_on_init(WxcApp* app, WxcOnInitCallback callback, void* arg) {
    app->_on_init = callback;
    app->_on_init_arg = arg;
}

static WxcApp* g_app;

wxAppConsole* _wxc_initialize() {
    return g_app;
}

void wxc_app_run(WxcApp* app) {
    wxApp::SetInitializerFunction(_wxc_initialize);
    int argc = 1;
    char** argv = new char*[1];
    argv[0] = new char[4];
    argv[0][1] = 'a';
    argv[0][2] = 0;
    wxEntry(argc, argv);
}
