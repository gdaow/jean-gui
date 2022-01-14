#include <wx/app.h>
#include "wxc/app.h"

struct WxcApp : public wxApp {
    virtual bool OnInit() override;
    WxcOnInitCallback _on_init = nullptr;
    void* _on_init_arg = nullptr;
};
