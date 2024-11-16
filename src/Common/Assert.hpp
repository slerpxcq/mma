#pragma once

#include <cassert>

#define MM_CORE_UNREACHABLE() MM_CORE_ASSERT(0 && "Unreachable")
#define MM_APP_UNREACHABLE() MM_APP_ASSERT(0 && "Unreachable")

#ifdef MM_CORE_DEBUG
#define MM_CORE_ASSERT(x) \
if (!(x)) { \
	MM_CORE_FATAL("Assertion failed: {0}", #x); \
	MM_CORE_FATAL("file={0}, func={1}, line={2}", __FILE__, __FUNCTION__, __LINE__); \
	__debugbreak(); }
#define MM_CORE_UNINPLEMENTED(x) MM_CORE_ASSERT(0 && "Unimplemented: " #x)
#else
#define MM_CORE_ASSERT(x) 
#define MM_CORE_UNINPLEMENTED(x) 
#endif

#ifdef MM_APP_DEBUG
#define MM_APP_UNINPLEMENTED(x) MM_APP_ASSERT(0 && "Unimplemented: " #x)
#define MM_APP_ASSERT(x) \
if (!(x)) { \
	MM_APP_FATAL("Assertion failed: {0}", #x); \
	MM_APP_FATAL("file={0}, func={1}, line={2}", __FILE__, __FUNCTION__, __LINE__); \
	__debugbreak(); }
#else
#define MM_APP_ASSERT(x) 
#define MM_APP_UNINPLEMENTED(x) 
#endif
