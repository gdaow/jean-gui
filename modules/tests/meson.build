cmocka_project = subproject('cmocka')
cmocka_dep = cmocka_project.get_variable('cmocka_dep')

jg_test_src = files(
    'main.c',
    'tests/containers/_index.c',
    'tests/containers/test_index.c',
    'tests/containers/test_vector.c',
    'tests/object-model/_index.c',
    'tests/object-model/test_value.c',
)

executable(
    'jg-tests',
    jg_test_src,
    dependencies: [jg_dep, cmocka_dep],
    include_directories: ['..']
)
