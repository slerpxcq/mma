#pragma once

#include "Log.hpp"

#include <chrono>

#define MM_GL_VERSION_MAJOR 4
#define MM_GL_VERSION_MINOR 6

#ifdef MM_DEBUG
#define MM_ASSERT(_expr_) do { \
	if (!(_expr_)) {\
		MM_FATAL("Assertion failed: " #_expr_ "\nFile: {0}\nFunction: {1}\nLine: {2}\n", __FILE__, __FUNCTION__, __LINE__);\
		__debugbreak();\
	}} while (false)
#else
#define MM_ASSERT(x) 
#endif

#define MM_TIME_NOW() std::chrono::high_resolution_clock::now()
#define MM_TIMEPOINT decltype(MM_TIME_NOW())
#define MM_TIME_DELTA(x) (std::chrono::duration_cast<std::chrono::microseconds>(MM_TIME_NOW() - (x)).count() * 1e-6f)

//#define MM_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#define MM_CALLBACK(x) &x, this
