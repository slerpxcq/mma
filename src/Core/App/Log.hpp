#pragma once

#include <spdlog/spdlog.h>

namespace mm
{
	class Logger {
	public:
		static std::shared_ptr<spdlog::logger> Get() {
			static std::shared_ptr<spdlog::logger> logger = spdlog::default_logger();
			return logger;
		}

	private:
		Logger() {}
	};
}

// TODO: Debug levels
#ifdef MM_DEBUG
#define MM_DBG(...) Logger::Get()->debug(__VA_ARGS__)
#define MM_INFO(...) Logger::Get()->info(__VA_ARGS__)
#define MM_WARN(...) Logger::Get()->warn(__VA_ARGS__)
#define MM_ERROR(...) Logger::Get()->error(__VA_ARGS__)
#define MM_FATAL(...) Logger::Get()->critical(__VA_ARGS__)
#else
#define MM_DBG(...) 
#define MM_INFO(...)
#define MM_WARN(...)
#define MM_ERROR(...)
#define MM_FATAL(...)
#endif
