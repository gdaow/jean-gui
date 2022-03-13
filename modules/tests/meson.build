cmocka_project = subproject('cmocka')
cmocka_dep = cmocka_project.get_variable('cmocka_dep')

jg_test_src = files(
    'common/fixtures/user_model.c',
    'main.c',
    'tests/containers/_index.c',
    'tests/containers/test_index.c',
    'tests/containers/test_vector.c',
    'tests/core/_index.c',
    'tests/core/test_context.c',
    'tests/core/test_id.c',
    'tests/object-model/_index.c',
    'tests/object-model/test_arguments.c',
    'tests/object-model/test_class.c',
    'tests/object-model/test_member.c',
    'tests/object-model/test_module.c',
    'tests/object-model/test_value.c',
)

unit_tests = executable(
    'jg-tests',
    jg_test_src,
    dependencies: [jg_dep, cmocka_dep],
    include_directories: ['..']
)

test('all-tests', unit_tests)

iwyu = find_program('iwyu_tool', required: false)
if iwyu.found()
    iwyu_target = run_target(
        'iwyu',
        command: [
            iwyu,
            '-p', meson.build_root(),
            meson.source_root() + '/modules'
        ]
    )
endif

