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
            'CMOCKA_MESSAGE_OUTPUT': 'TAP'
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

iwyu = find_program('iwyu_tool', required: false)
if iwyu.found()
    iwyu_target = run_target(
        'iwyu',
        command: [
            iwyu,
            '-p', meson.build_root(),
            meson.source_root() + '/src',
            meson.source_root() + '/tests',
            meson.source_root() + '/tools'
        ]
    )
endif

