/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 * 
 */
#include <wx/wx.h>

/*
#include <jgui/module.h>
#include <jgui/object.h>
#include <jgui/core.h>

#include <jgui/wx/app.h>

struct jwx_app_s : public wxApp {
    ~jwx_app_s() override = default;
    bool OnInit() override;
};

bool jwx_app_s::OnInit() {
    return true;
}

static jg_value initialize(jg_arguments* args) {
    void* object = jg_pop_object(args);
    new (object) jwx_app_s();
    return jg_none();
}

static jg_value cleanup(jg_arguments* args) {
    auto* app = static_cast<jwx_app_s*>(jg_pop_object(args));
    app->~jwx_app_s();
    return jg_none();
}

void jwx_app_class_register(jg_module_builder* module) {
    jg_class_builder* class_ = jg_module_add_class(
        module,
        jwx_app_class_id,
        jg_core_ns,
        jg_object_class_id,
        sizeof(jwx_app)
    );

    jg_class_add_method(class_, "initialize", initialize);
    jg_class_add_method(class_, "cleanup", cleanup);
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
*/