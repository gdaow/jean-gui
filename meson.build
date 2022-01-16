project('uiligi', 'c')

wxc = dependency('wxc', required : true)

cmake = import('cmake')

yaml_project = cmake.subproject('libyaml')
yaml = yaml_project.dependency('yaml')

ulg_includes = include_directories('include')

ulg_lib = static_library(
    'uiligi',
    'src/context.c',
    'src/object.c',
    'src/template.c',
    'src/utils/hashmap.c',
    'src/value.c',
    dependencies: [wxc, yaml],
    include_directories: ulg_includes
)

ulg_dep = declare_dependency(
    include_directories: ulg_includes,
    link_with: ulg_lib
)

check_project = cmake.subproject('check')
check = check_project.dependency('check')

executable(
    'uiligi-tests',
    'tests/fixtures/admin.c',
    'tests/fixtures/user.c',
    'tests/main.c',
    'tests/test_object.c',
    dependencies: [ulg_dep, check],
)
