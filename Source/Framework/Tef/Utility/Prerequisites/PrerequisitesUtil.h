#pragma once

#include <assert.h>

// Platform-specific stuff
#include "PlatformDefines.h"

#if TE_COMPILER == TE_COMPILER_MSVC
// disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#	pragma warning (disable: 4251)

// disable: 'X' Function call with parameters that may be unsafe
#	pragma warning(disable: 4996)

// disable: decorated name length exceeded, name was truncated
// Happens with really long type names. Even fairly standard use
// of std::unordered_map with custom parameters, meaning I can't
// really do much to avoid it. It shouldn't effect execution
// but might cause problems if you compile library
// with one compiler and use it in another.
#	pragma warning(disable: 4503)

// disable: C++ exception handler used, but unwind semantics are not enabled
// We don't care about this as any exception is meant to crash the program.
#	pragma warning(disable: 4530)
#endif

// Windows Settings
#if TE_PLATFORM == TE_PLATFORM_WIN32
// Win32 compilers use _DEBUG for specifying debug builds.
// for MinGW, we set DEBUG
#   if defined(_DEBUG) || defined(DEBUG)
#       define TE_DEBUG_MODE 1
#   else
#       define TE_DEBUG_MODE 0
#   endif
#endif

// Linux/Apple Settings
#if TE_PLATFORM == TE_PLATFORM_LINUX || TE_PLATFORM == TE_PLATFORM_OSX
// A quick define to overcome different names for the same function
#   define stricmp strcasecmp

#   ifdef DEBUG
#       define TE_DEBUG_MODE 1
#   else
#       define TE_DEBUG_MODE 0
#   endif
#endif

#if TE_DEBUG_MODE
#   define TE_DEBUG_ONLY(x) x
#   define TE_ASSERT(x) assert(x)
#else
#   define TE_DEBUG_ONLY(x)
#   define TE_ASSERT(x)
#endif

#include "Prerequisites/FwdDeclUtil.h"
#include "Prerequisites/Types.h"
#include "Allocators/MemoryAllocator.h"
#include "Prerequisites/StdHeaders.h"
#include "Debug/Debug.h"
#include "Error/Error.h"
#include "String/String.h"
#include "Utility/NonCopyable.h"
#include "Threading/Threading.h"