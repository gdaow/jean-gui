llvm_dep = dependency('llvm', version: '>= 3.5')

clang_include = llvm_dep.get_configtool_variable('includedir')
clang_libdir = llvm_dep.get_configtool_variable('libdir').split(' ')

compiler = meson.get_compiler('cpp')
if not compiler.has_header('clang-c/Index.h', args: '-I' + clang_include)
    error('Failed to find clang headers')
endif

clang_dep = declare_dependency(
    dependencies: compiler.find_library('libclang', dirs: clang_libdir),
    include_directories: include_directories(clang_include),
)

wx_bind = executable(
    'wx-bind',
    'src/main.cc',
    dependencies: clang_dep
)
