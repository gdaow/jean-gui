#include <stdlib.h>
#include <yaml.h>

#include "wxligi/template.h"

#include "wxligi/context.h"

struct WlgTemplate {
    WlgContext* context;
};

WlgTemplate* wlg_template_from_string(WlgContext* context, const char* source) {
    return malloc(sizeof(WlgTemplate));
}

void wlg_template_instanciate(WlgTemplate* template) {

}