add_library(
	jgui

	private/containers/index.c
	private/containers/vector.c
	private/core/context.c
	private/core/core-plugin.c
	private/core/id.c
	private/memory/alloc.c
	private/memory/string.c
	private/misc/assert.c
	private/object-model/arguments.c
	private/object-model/class.c
	private/object-model/member.c
	private/object-model/module.c
	private/object-model/value.c
)

target_include_directories(
	jgui

	PUBLIC include
	PRIVATE ..
)

jgui_target(jgui)

