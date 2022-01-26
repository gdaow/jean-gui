project(
    'mezo',
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

mz_includes = include_directories('include')

mz_src = files(
    'src/object.c',
    'src/private/memory.c',
    'src/private/stack.c',
    'src/private/misc.c',
    'src/template.c',
    'src/value.c',
)

mz_lib = static_library(
    'mezo',
    mz_src,
    dependencies: [wxc, yaml],
    include_directories: mz_includes,
    c_args: ['-Wall', '-Wextra', '-Wpedantic', '-Wconversion',  '-fno-strict-aliasing']
)

mz_dep = declare_dependency(
    include_directories: mz_includes,
    link_with: mz_lib
)

minunit_includes = include_directories('subprojects/minunit')

mz_test_src = files(
    'tests/fixtures/user_model.c',
    'tests/main.c',
    'tests/test_object.c',
    'tests/test_template.c',
    'tests/test_value.c',
)

executable(
    'mezo-tests',
    mz_test_src,
    dependencies: [mz_dep],
    include_directories: [minunit_includes]
)

clangtidy = find_program('clang-tidy', required: false)
if clangtidy.found()
    run_target(
        'tidy',
        command: [
            clangtidy,
            '-p', meson.build_root()
        ] + mz_src)
endif
