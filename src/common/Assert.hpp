#pragma once

#include <cassert>

#define MM_UNREACHABLE() MM_ASSERT(0 && "Unreachable")

#ifdef MM_DEBUG
// TODO: add implementation
#define MM_ASSERT(x) assert(x)
#define MM_UNINPLEMENTED(x) MM_ASSERT(0 && "Unimplemented: " x)
#else
#define MM_ASSERT(x) 
#define MM_UNINPLEMENTED(x) 
#endif
