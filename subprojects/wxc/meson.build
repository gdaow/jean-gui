project('wxc', 'cpp')
wx_widgets = dependency('wxwidgets', required : true)
static_library(
    'wxc',
    'src/app.cc',
    'src/frame.cc',
    'src/msgdlg.cc',
    dependencies: wx_widgets
)
