#include "wxc/app.h"
#include "wx/wx.h"

struct wxcApp : public wxApp {
    virtual bool OnInit() override;
    wxcOnInitCallback _on_init = nullptr;
    void* _on_init_arg = nullptr;
};

bool wxcApp::OnInit() {
    if(_on_init) {
        return (*_on_init)(_on_init_arg);
    }
    return wxApp::OnInit();
}

wxcApp* wxcApp_Create() {
    return new wxcApp();
}

void wxcApp_OnInit(wxcApp* app, wxcOnInitCallback callback, void* arg) {
    app->_on_init = callback;
    app->_on_init_arg = arg;
}

static wxcApp* globalApp;

wxAppConsole* wxcInitialize() {
    return globalApp;
}

void wxcRun(wxcApp* app) {
    wxApp::SetInitializerFunction(wxcInitialize);
    int argc = 1;
    char** argv = new char*[1];
    argv[0] = new char[4];
    argv[0][1] = 'a';
    argv[0][2] = 0;
    wxEntry(argc, argv);
}
