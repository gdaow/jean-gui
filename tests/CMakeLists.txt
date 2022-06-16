
function(test test_directory test_name)
	add_executable(${test_name} "tests/${test_directory}/${test_name}.c")
	target_include_directories(${test_name} PRIVATE ../src .)
	target_link_libraries(${test_name} cmocka jgui)
	add_test(NAME ${test_name} COMMAND ${test_name})
endfunction(add_test)

test(containers vector-tests)
