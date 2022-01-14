project('wxc',
    'cpp',
    default_options : ['cpp_std=c++14']
)
wx_widgets = dependency('wxwidgets', required : true)
wxc_includes = include_directories('include')
wxc_lib = static_library(
    'wxc',
    'src/wxc_app.cc',
    'src/frame.cc',
    'src/msgdlg.cc',
    dependencies: wx_widgets,
    include_directories: wxc_includes
)

wxc_dep = declare_dependency(
    include_directories: wxc_includes,
    link_with: wxc_lib
)

meson.override_dependency('wxc', wxc_dep)
