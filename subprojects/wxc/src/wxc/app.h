typedef bool (*wxcOnInitCallback)(void* arg);

extern "C" {
    struct wxcApp;

    wxcApp* wxcApp_Create();
    void wxcApp_OnInit(wxcApp* app, wxcOnInitCallback callback, void* arg);
    void wxcRun(wxcApp* app);
}
