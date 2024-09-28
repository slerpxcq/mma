#pragma once

#include "Base.hpp"

#define FORWARD_IMPL(_name_) \
template <typename... Args> \
void _name_(Args&&... args) { m_impl._name_(std::forward<Args>(args)...); }

namespace mm
{

template <typename Impl>
class Log 
{
	MM_DECL_SINGLETON(Log)
public:
	FORWARD_IMPL(Trace)
	FORWARD_IMPL(Info)
	FORWARD_IMPL(Warn)
	FORWARD_IMPL(Error)
	FORWARD_IMPL(Fatal)

private:
	Impl m_impl{};
};

}

#undef FORWARD_IMPL

