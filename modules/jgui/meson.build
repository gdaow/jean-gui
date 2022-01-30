cmake = import('cmake')

yaml_project = cmake.subproject(
    'libyaml',
    override_options: [
        'warning_level=0',
        'werror=false',
    ]
)

yaml = yaml_project.dependency('yaml', include_type: 'system')

jg_includes = include_directories('public')

jg_src = files(
    'private/common/arena.c',
    'private/common/debug.c',
    'private/common/index.c',
    'private/common/memory.c',
    'private/common/stack.c',
    'private/common/string.c',
    'private/class.c',
    'private/context.c',
    'private/core_plugin.c',
    'private/module.c',
    'private/object.c',
    'private/template.c',
    'private/value.c',
)

jg_lib = static_library(
    'jgui',
    jg_src,
    dependencies: [yaml],
    include_directories: jg_includes,
    c_args: ['-Wall', '-Wextra', '-Wpedantic', '-Wconversion',  '-fno-strict-aliasing']
)

jg_dep = declare_dependency(
    include_directories: jg_includes,
    link_with: jg_lib
)
