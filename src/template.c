#include <stdlib.h>
#include <yaml.h>
#include "wxligi/template.h"
#include "wxligi/app.h"

struct WlgTemplate {

};

WlgTemplate* wlg_template_from_string(WlgApp* app, const char* source) {
    return malloc(sizeof(WlgTemplate));
}

void wlg_template_instanciate(WlgTemplate* template) {

}