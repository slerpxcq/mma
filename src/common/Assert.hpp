#pragma once

#include <cassert>

#define MM_CORE_UNREACHABLE() MM_CORE_ASSERT(0 && "Unreachable")

#ifdef MM_CORE_DEBUG
// TODO: add log implementation
#define MM_CORE_ASSERT(x) assert(x)
#define MM_CORE_UNINPLEMENTED(x) MM_CORE_ASSERT(0 && "Unimplemented: " x)
#else
#define MM_CORE_ASSERT(x) 
#define MM_CORE_UNINPLEMENTED(x) 
#endif

#ifdef MM_APP_DEBUG
// TODO: add log implementation
#define MM_APP_ASSERT(x) assert(x)
#define MM_APP_UNINPLEMENTED(x) MM_APP_ASSERT(0 && "Unimplemented: " x)
#else
#define MM_APP_ASSERT(x) 
#define MM_APP_UNINPLEMENTED(x) 
#endif
