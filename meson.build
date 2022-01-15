project('wxligi', 'c')


wxc = dependency('wxc', required : true)

cmake = import('cmake')

yaml_project = cmake.subproject('libyaml')
yaml = yaml_project.dependency('yaml')

wlg_includes = include_directories('include')

executable(
    'wxligi',
    'src/context.c',
    'src/template.c',
    'src/test.c',
    'src/utils/hashmap.c',
    dependencies: [wxc, yaml],
    include_directories: wlg_includes
)

