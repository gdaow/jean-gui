#include "wxc/common.h"

WXC_HEADER_START

typedef struct WxcApp WxcApp;

typedef bool (*WxcOnInitCallback)(WxcApp* app, void*);

WxcApp* wxc_app_create();
void wxc_app_destroy(WxcApp* app);
void wxc_app_on_init(WxcApp* app, WxcOnInitCallback callback, void* arg);
bool wxc_app_on_init_parent(WxcApp* app);
void wxc_app_run(WxcApp* app);

WXC_HEADER_END
