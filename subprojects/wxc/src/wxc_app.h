#include <wx/app.h>
#include "wxc/app.h"
#include "common/closure.h"

struct WxcApp : public wxApp {
    virtual bool OnInit() override;

    Closure<WxcOnInitCallback> on_init;
};
