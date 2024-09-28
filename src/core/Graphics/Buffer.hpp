#pragma once

#include "GPUResource.hpp"

namespace mm
{

class Buffer : public GPUResource
{
public:
	enum class Target {
		VERTEX, INDEX
	};

public:
	Buffer(Target target) : m_target{target} {}
	virtual ~Buffer() = default;

	virtual void SetData(void* data, u32 size) = 0;
	virtual void SetSubData(void* data, u32 size, u32 offset) = 0;
	virtual void SetBindBase(u32 base) = 0;

protected:
	u32 m_size{};
	Target m_target{};
};

}

