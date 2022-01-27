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

jg_includes = include_directories('include')

jg_src = files(
    'src/object.c',
    'src/private/memory.c',
    'src/private/stack.c',
    'src/private/misc.c',
    'src/template.c',
    'src/value.c',
)

jg_lib = static_library(
    'jg',
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
    'tests/fixtures/user_model.c',
    'tests/main.c',
    'tests/test_object.c',
    'tests/test_template.c',
    'tests/test_value.c',
)

executable(
    'jg-tests',
    jg_test_src,
    dependencies: [jg_dep],
    include_directories: [minunit_includes]
)

clangtidy = find_program('clang-tidy', required: false)
if clangtidy.found()
    run_target(
        'tidy',
        command: [
            clangtidy,
            '-p', meson.build_root()
        ] + jg_src)
endif
