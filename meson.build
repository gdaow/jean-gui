project('jg', 'c', 'cpp', default_options: [
    'warning_level=3',
    'werror=true',
    'c_std=c2x',
    'cpp_std=c++2a'
])

subdir('modules/jgui')
subdir('modules/jgui-wx')
subdir('modules/samples')
subdir('modules/tests')

clangtidy = find_program('clang-tidy', required: false)
if clangtidy.found()
    run_target(
        'tidy',
        command: [
            clangtidy,
            '-p', meson.build_root()
        ] + jg_src)
endif
