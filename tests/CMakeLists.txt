add_executable(test-vector tests/containers/vector-tests.c)
target_include_directories(test-vector PRIVATE ../src .)
target_link_libraries(test-vector cmocka jgui)
add_test(NAME test-vector COMMAND test-vector)
