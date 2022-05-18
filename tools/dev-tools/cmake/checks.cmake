add_custom_target(clang-format)
add_custom_target(clang-tidy)
add_custom_target(cppcheck)
add_custom_target(iwyu)
add_custom_target(static-checks)
add_custom_target(format)
add_dependencies(static-checks clang-format clang-tidy cppcheck iwyu)

function(check target)
	clang_format(${target})
	clang_tidy(${target})
	cppcheck(${target})
	iwyu(${target})
endfunction()

function(clang_format target)
	find_program(CLANG_FORMAT clang-format-12)
	if(CLANG_TIDY)
		add_check_target(${target} clang-format clang-format-12 --dry-run -Werror)
		add_check_target(${target} format clang-format-12 -i)
	endif()
endfunction()

function(clang_tidy target)
	find_program(CLANG_TIDY clang-tidy)
	if(CLANG_TIDY)
		add_check_target(${target} clang-tidy clang-tidy --quiet -p ${CMAKE_BINARY_DIR})
	endif()
endfunction()

function(cppcheck target)
	find_program(CPPCHECK cppcheck)
	if(CPPCHECK)
		add_check_target(${target} cppcheck cppcheck --enable=all --suppress=unusedFunction --suppress=missingInclude)
	endif()
endfunction()

function(iwyu target)
	find_program(IWYU iwyu)
	if(IWYU)
		add_check_target(${target} iwyu python3 ${CMAKE_SOURCE_DIR}/tools/dev-tools/iwyu/iwyu_tool.py -p ${CMAKE_BINARY_DIR} -o clang)
	endif()
endfunction()

function(add_check_target target tool_name)
	get_target_property(check_sources ${target} SOURCES)
	set(check_target ${target}_${tool_name})

	set(check_commands)
	add_custom_target(${check_target})

	foreach(source_file ${check_sources})
		if( source_file MATCHES \\.c$ )
			get_filename_component(absolute_source_file ${source_file} ABSOLUTE)
			add_custom_command(
				TARGET ${check_target}
				PRE_BUILD
				COMMAND ${ARGN} ${absolute_source_file}
				COMMENT "${tool_name} ${source_file}"
				VERBATIM
			)
		endif()
	endforeach(source_file)

	add_dependencies(${tool_name} ${check_target})
endfunction(add_check_target)

