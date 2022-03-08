cmake = import('cmake')

opt_var = cmake.subproject_options()
opt_var.set_override_option('warning_level', '0')
opt_var.set_override_option('werror', 'false')

yaml_project = cmake.subproject('libyaml')

yaml = yaml_project.dependency('yaml', include_type: 'system')

jg_includes = include_directories('include')
jg_private_includes = include_directories('..')

jg_src = files(
    'private/containers/index.c',
    'private/containers/stack.c',
    'private/containers/vector.c',
    'private/core/context.c',
    'private/core/core_plugin.c',
    'private/memory/alloc.c',
    'private/memory/arena.c',
    'private/memory/string.c',
    'private/misc/assert.c',
    'private/object-model/arguments.c',
    'private/object-model/class.c',
    'private/object-model/member.c',
    'private/object-model/module.c',
    'private/object-model/object.c',
    'private/object-model/value.c',
    'private/templating/template.c',
)

jg_lib = static_library(
    'jgui',
    jg_src,
    dependencies: [yaml],
    include_directories: [jg_includes, jg_private_includes],
    c_args: ['-Wall', '-Wextra', '-Wpedantic', '-Wconversion',  '-fno-strict-aliasing'],
    override_options: [
      'warning_level=3',
      'werror=true',
    ]
)

jg_dep = declare_dependency(
    include_directories: jg_includes,
    link_with: jg_lib
)
