#pragma once

#include "Log.hpp"
#include <spdlog/spdlog.h>

#include <memory>

#define FORWARD_SPDLOG(_func_, _spdfunc_) \
template <typename... Args> \
void _func_(Args&&... args) { m_logger->_spdfunc_(std::forward<Args>(args)...); }

namespace mm
{

class LogImpl_spdlog
{
public:
	LogImpl_spdlog(const char* name) :
		m_logger{ spdlog::default_logger()->clone(name) } {
		m_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v"); 
		m_logger->set_level(spdlog::level::trace);
	}

	FORWARD_SPDLOG(Trace, trace)
	FORWARD_SPDLOG(Info, info)
	FORWARD_SPDLOG(Warn, warn)
	FORWARD_SPDLOG(Error, error)
	FORWARD_SPDLOG(Fatal, critical)

private:
	std::shared_ptr<spdlog::logger> m_logger;
};

}

#undef FORWARD_SPDLOG

