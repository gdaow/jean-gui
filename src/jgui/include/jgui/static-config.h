/**
 * Copyright © 2022 Corentin Séchet <corentin@ki-dour.org>
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 * TODO:
 *  - document.
 *  - rename to config.h, runtime config is gone
 *  - maybe merge with the lonely utils.h ?
 */
#ifndef JGUI_PUBLIC_JGUI_STATIC_CONFIG_H
#define JGUI_PUBLIC_JGUI_STATIC_CONFIG_H

#define JG_ARENA_DEFAULT_CHUNK_SIZE 0x1000
#define JG_MAX_IDENTIFIER_LENGTH 256
#define JG_MAX_ERROR_MESSAGE_LENGTH 0x1000

#ifdef NDEBUG
#	define JG_DEBUG 0
#else
#	define JG_DEBUG 1
#endif

#if JG_DEBUG
// Memory debugging flags
#	define JG_MEMORY_DEBUG 1
#	define JG_IF_MEMORY_DEBUG(...) __VA_ARGS__
#	define JG_ENABLE_ASSERTS 1

#else
#	define JG_MEMORY_DEBUG 0
#	define JG_IF_MEMORY_DEBUG(...)
#	define JG_ENABLE_ASSERTS 0
#endif

#endif
