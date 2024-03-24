#pragma once

#define MM_GL_VERSION_MAJOR 4
#define MM_GL_VERSION_MINOR 3

// TODO: Debug levels
#ifdef MM_DEBUG
#define MM_DBG(...) spdlog::debug(__VA_ARGS__)
#define MM_INFO(...) spdlog::info(__VA_ARGS__)
#define MM_WARN(...) spdlog::warn(__VA_ARGS__)
#define MM_ERROR(...) spdlog::error(__VA_ARGS__)
#define MM_FATAL(...) spdlog::critical(__VA_ARGS__)
#else
#define MM_DBG(...) 
#define MM_INFO(...)
#define MM_WARN(...)
#define MM_ERROR(...)
#define MM_FATAL(...)
#endif

// TODO: Assert enable
#ifdef MM_DEBUG
#define MM_ASSERT(x) do { \
	if (!(x)) {\
		MM_FATAL("Assertion failed:\n  File: {0}\n  Function: {1}\n  Line: {2}\n", __FILE__, __FUNCTION__, __LINE__);\
		__debugbreak();\
	}} while (false)
#else
#define MM_ASSERT(x) 
#endif

#define MM_TIMEPOINT decltype(std::chrono::high_resolution_clock::now())
#define MM_TIME_NOW() std::chrono::high_resolution_clock::now()
#define MM_TIME_DELTA(x) (std::chrono::duration_cast<std::chrono::microseconds>(MM_TIME_NOW() - (x)).count() * 0.000001f)

#define MM_WRAP(x) [&](){x;}

#define MM_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#define MM_SKINNING_BASE 0
#define MM_MATERIAL_BASE 1



