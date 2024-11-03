#pragma once

#include "Base.hpp"

/* include implementation header here*/
#include "LogImpl_spdlog.hpp"

#define FORWARD_IMPL(_name_) \
template <typename... Args> \
void _name_(Args&&... args) { m_impl._name_(std::forward<Args>(args)...); }

#define MM_CORE_TRACE(...) GetCoreLogger()->Trace(__VA_ARGS__)
#define MM_CORE_INFO(...) GetCoreLogger()->Info(__VA_ARGS__)
#define MM_CORE_WARN(...) GetCoreLogger()->Warn(__VA_ARGS__)
#define MM_CORE_ERROR(...) GetCoreLogger()->Error(__VA_ARGS__)
#define MM_CORE_FATAL(...) GetCoreLogger()->Fatal(__VA_ARGS__)

#define MM_APP_TRACE(...) GetAppLogger()->Trace(__VA_ARGS__)
#define MM_APP_INFO(...) GetAppLogger()->Info(__VA_ARGS__)
#define MM_APP_WARN(...) GetAppLogger()->Warn(__VA_ARGS__)
#define MM_APP_ERROR(...) GetAppLogger()->Error(__VA_ARGS__)
#define MM_APP_FATAL(...) GetAppLogger()->Fatal(__VA_ARGS__)

namespace mm
{

template <typename Impl>
class Log 
{
public:
	Log(const char* name = "") : m_impl{name} {}

	FORWARD_IMPL(Trace)
	FORWARD_IMPL(Info)
	FORWARD_IMPL(Warn)
	FORWARD_IMPL(Error)
	FORWARD_IMPL(Fatal)

private:
	Impl m_impl{};
};

/* define implementation here */
using Logger = Log<LogImpl_spdlog>;

}

#undef FORWARD_IMPL

