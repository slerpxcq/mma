#pragma once

#include "GPUResource.hpp"

namespace mm
{

class Buffer : public GPUResource
{
public:
	enum class Target {
		VERTEX, INDEX, SHADER_STORAGE
	};

public:
	Buffer(Target target);
	virtual ~Buffer() = default;

	Target GetTarget() const { return m_target; }
	void SetData(void* data, u32 size) { GetGraphics()->SetBufferData(*this, data, size); }
	void SetSubData(void* data, u32 size, u32 offset) { GetGraphics()->SetBufferSubData(*this, data, size, offset); }
	void SetBindBase(u32 base) { GetGraphics()->SetBufferBindBase(*this, base); }

private:
	Target m_target{};
	u32 m_size{};
};

}

