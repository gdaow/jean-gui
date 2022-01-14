#include "wxc/common.h"

WXC_HEADER_START

typedef bool (*WxcOnInitCallback)(void*);
typedef struct WxcApp WxcApp;

WxcApp* wxc_app_create();
void wxc_app_destroy(WxcApp* app);
void wxc_app_on_init(WxcApp* app, WxcOnInitCallback callback, void* arg);
void wxc_app_run(WxcApp* app);

WXC_HEADER_END
