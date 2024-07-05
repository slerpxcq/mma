#pragma once

#include <spdlog/spdlog.h>

/* This component is the only one has static lifetime, therefore is 
 * not controlled by Application */

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