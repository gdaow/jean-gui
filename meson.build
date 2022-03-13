project('jg', 'c', 'cpp', default_options: [
    'c_std=c2x',
    'cpp_std=c++2a'
])

subdir('src/jgui')
subdir('tests')

clangtidy = find_program('clang-tidy', required: false)
if clangtidy.found()
    run_target(
        'tidy',
        command: [
            clangtidy,
            '-p', meson.build_root()
        ] + jg_src)
endif
