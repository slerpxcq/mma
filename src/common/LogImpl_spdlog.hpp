#pragma once

#include "Log.hpp"
#include <spdlog/spdlog.h>

#define FORWARD_SPDLOG(_func_, _spdfunc_) \
template <typename... Args> \
void _func_(Args&&... args) { _spdfunc_(std::forward<Args>(args)...); }

namespace mm
{

class LogImpl_spdlog
{
public:
	FORWARD_SPDLOG(Trace, spdlog::trace)
	FORWARD_SPDLOG(Info, spdlog::info)
	FORWARD_SPDLOG(Warn, spdlog::warn)
	FORWARD_SPDLOG(Error, spdlog::error)
	FORWARD_SPDLOG(Fatal, spdlog::critical)
};

}

#undef FORWARD_SPDLOG

