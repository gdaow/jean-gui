project(
    'jg',
    'c',
    default_options: [
        'warning_level=3',
        'werror=true',
        'c_std=c2x'
    ])

wxc = dependency(
    'wxc',
    required : true,
    default_options: [
        'warning_level=0',
        'werror=false'
    ]
)

cmake = import('cmake')

yaml_project = cmake.subproject(
    'libyaml',
    default_options: [
        'warning_level=0',
        'werror=false',
    ]
)
yaml = yaml_project.dependency('yaml')

jg_includes = include_directories('modules/jgui/public')

jg_src = files(
    'modules/jgui/private/common/arena.c',
    'modules/jgui/private/common/debug.c',
    'modules/jgui/private/common/index.c',
    'modules/jgui/private/common/stack.c',
    'modules/jgui/private/common/string.c',
    'modules/jgui/private/class.c',
    'modules/jgui/private/context.c',
    'modules/jgui/private/module.c',
    'modules/jgui/private/object.c',
    'modules/jgui/private/template.c',
    'modules/jgui/private/value.c',
)

jg_lib = static_library(
    'jgui',
    jg_src,
    dependencies: [wxc, yaml],
    include_directories: jg_includes,
    c_args: ['-Wall', '-Wextra', '-Wpedantic', '-Wconversion',  '-fno-strict-aliasing']
)

jg_dep = declare_dependency(
    include_directories: jg_includes,
    link_with: jg_lib
)

minunit_includes = include_directories('subprojects/minunit')

jg_test_src = files(
    'modules/tests/fixtures/user_model.c',
    'modules/tests/main.c',
    'modules/tests/test_context.c',
    'modules/tests/test_object.c',
    'modules/tests/test_template.c',
    'modules/tests/test_value.c',
)

executable(
    'jg-tests',
    jg_test_src,
    dependencies: [jg_dep],
    include_directories: [minunit_includes]
)

executable('jg-sample', 'modules/samples/main.c', dependencies: [jg_dep])

clangtidy = find_program('clang-tidy', required: false)
if clangtidy.found()
    run_target(
        'tidy',
        command: [
            clangtidy,
            '-p', meson.build_root()
        ] + jg_src)
endif
