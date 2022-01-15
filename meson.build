project('wxligi', 'c')
wxc = dependency('wxc', required : true)
wlg_includes = include_directories('include')
executable(
    'wxligi',
    'src/test.c',
    'src/utils/hashmap.c',
    dependencies: wxc,
    include_directories: wlg_includes
)

