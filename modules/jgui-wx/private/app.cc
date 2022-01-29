/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 */
#include <wx/wx.h>

#include <jgui/module.h>
#include <jgui/object.h>

#include <jgui/wx/app.h>

class jwx_app_s : public wxApp {
    ~jwx_app_s() override = default;
    bool OnInit() override;
};

bool jwx_app_s::OnInit() {
    return true;
}

void jwx_app_class_register(jg_module_definition* module) {
    jg_module_add_class(
        module,
        jwx_app_class_id,
        jg_object_class_id,
        sizeof(jwx_app),
        alignof(jwx_app)
    );
}

static jwx_app* g_app;
static wxAppConsole* jwx_initialize() {
    return g_app;
}

void wxc_app_run(jwx_app* app) {
    g_app = app;
    wxApp::SetInitializerFunction(jwx_initialize);
    int argc = 1;
    char** argv = new char*[1];
    argv[0] = new char[4];
    argv[0][1] = 'a';
    argv[0][2] = 0;
    wxEntry(argc, argv);
}
