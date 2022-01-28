wx_widgets = dependency('wxwidgets', required : true, include_type: 'system')

jwx_includes = include_directories('public')

jwx_lib = static_library(
    'jwx',
    'private/app.cc',
    dependencies: wx_widgets,
    include_directories: [jwx_includes, jg_includes]
)

jwx_dep = declare_dependency(
    include_directories: jwx_includes,
    link_with: jwx_lib
)

meson.override_dependency('jgui-wx', jwx_dep)
