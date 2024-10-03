#pragma once

#include "Base.hpp"

#include "LogImpl_spdlog.hpp"

#define FORWARD_IMPL(_name_) \
template <typename... Args> \
void _name_(Args&&... args) { m_impl._name_(std::forward<Args>(args)...); }

#define MM_CORE_TRACE(...)
#define MM_CORE_INFO(...)
#define MM_CORE_WARN(...)
#define MM_CORE_ERROR(...)
#define MM_CORE_FATAL(...)

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

/* Change implementation here */
using Logger = Log<LogImpl_spdlog>;

}

#undef FORWARD_IMPL

