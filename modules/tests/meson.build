minunit_includes = include_directories('../../subprojects/minunit')

jg_test_src = files(
    'fixtures/user_model.c',
    'main.c',
    'test_context.c',
    'test_object.c',
    'test_template.c',
    'test_value.c',
)

executable(
    'jg-tests',
    jg_test_src,
    dependencies: [jg_dep],
    include_directories: [minunit_includes]
)
