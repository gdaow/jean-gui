test_suites = {
    'containers': {
        'index': 'containers/index-tests.c',
        'vector': 'containers/vector-tests.c'
    },
    'core' : {
        'context': 'core/context-tests.c',
        'id': 'core/id-tests.c',
    },
    'object-model': {
        'arguments': 'object-model/arguments-tests.c',
        'class': 'object-model/class-tests.c',
        'member': 'object-model/member-tests.c',
        'module': 'object-model/module-tests.c',
        'value': 'object-model/value-tests.c',
    }
}

valgrind = find_program('valgrind', required: false)

if valgrind.found()
    add_test_setup(
        'default',
        is_default: true,
        env: {
            'CMOCKA_MESSAGE_OUTPUT': 'TAP'
        },
        exe_wrapper: ['valgrind', '--leak-check=full', '--track-origins=yes', '--error-exitcode=1']
    )
else
    add_test_setup(
        'default',
        is_default: true,
        env: {
            'CMOCKA_MESSAGE_OUTPUT' : 'TAP'
        }
    )
endif

cmocka_project = subproject('cmocka')
cmocka_dep = cmocka_project.get_variable('cmocka_dep')

foreach suite_name, suite_config : test_suites
    foreach test_name, test_source : suite_config
        sources = files(
          'common/fixtures/user_model.c',
          'common/hooks.c',
          'tests/' + test_source
        )
        test_exe = executable(
          'test-' + suite_name + '-' + test_name,
          'common/fixtures/user_model.c',
          sources,
          dependencies: [jg_dep, cmocka_dep],
          include_directories: ['../src', '.']
        )
        test(test_name, test_exe, protocol: 'tap', suite: suite_name)
    endforeach
endforeach

iwyu_target = run_target(
    'iwyu',
    command: [
        'python3',
        meson.source_root() + '/tools/dev-tools/iwyu/iwyu_tool.py',
        '-p', meson.build_root(),
        '-o', 'clang',
        meson.source_root() + '/src',
        meson.source_root() + '/tests',
        '--', '-Xiwyu', '--mapping_file=' + meson.source_root() + '/.iwyu-mappings.imp'
    ]
)

