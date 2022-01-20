project(
    'uiligi',
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

ulg_includes = include_directories('include')

ulg_lib = static_library(
    'uiligi',
    'src/object.c',
    'src/template.c',
    'src/value.c',
    dependencies: [wxc, yaml],
    include_directories: ulg_includes,
    c_args: ['-Wall', '-Wextra', '-Wpedantic', '-Wconversion',  '-fno-strict-aliasing']
)

ulg_dep = declare_dependency(
    include_directories: ulg_includes,
    link_with: ulg_lib
)

minunit_includes = include_directories('subprojects/minunit')

executable(
    'uiligi-tests',
    'tests/fixtures/user_model.c',
    'tests/main.c',
    'tests/test_object.c',
    'tests/test_template.c',
    'tests/test_value.c',
    dependencies: [ulg_dep],
    include_directories: [minunit_includes]
)
