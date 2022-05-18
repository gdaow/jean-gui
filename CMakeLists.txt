cmake_minimum_required(VERSION 3.10)

project(Jean-Gui)

set(CMAKE_C_CPPCHECK "cppcheck" "--enable=all" "--suppress=unusedFunction")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/tools/dev-tools/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
include(checks)
include(iwyu)

enable_testing()

add_subdirectory(src/jgui)

add_subdirectory(subprojects/cmocka)

add_subdirectory(tests)
