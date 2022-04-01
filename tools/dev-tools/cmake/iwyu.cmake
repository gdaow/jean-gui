
add_custom_target(checks 
	"cmake"
	"-DENABLE_IWYU"
	"-DENABLE_CLANG_TIDY"
	"-DENABLE_CPPCHECK"
		"${CMAKE_SOURCE_DIR}/tools/dev-tools/iwyu/iwyu_tool.py"
		"-o" "clang"
		"-p" "${CMAKE_BUILD_DIRECTORY}"
		"**"
)
