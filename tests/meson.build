test_suites = {
    'containers': {
        'index': 'containers/test_index.c',
        'vector': 'containers/test_vector.c'
    },
    'core' : {
        'context': 'core/test_context.c',
        'id': 'core/test_id.c',
    },
    'object-model': {
        'arguments': 'object-model/test_arguments.c',
        'class': 'object-model/test_class.c',
        'member': 'object-model/test_member.c',
        'module': 'object-model/test_module.c',
        'value': 'object-model/test_value.c',
    }
}

add_test_setup('default', is_default: true, env: {'CMOCKA_MESSAGE_OUTPUT': 'TAP' })

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

