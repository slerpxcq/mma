#pragma once

#include "GPUResource.hpp"

namespace mm
{

class Buffer : public GPUResource
{
public:
	enum class Target {
		VERTEX, INDEX, UNIFORM, SHADER_STORAGE
	};

public:
	virtual ~Buffer() { GetGraphics()->DeleteBuffer(*this); }
	Target GetTarget() const { return m_target; }

protected:
	Buffer(Target target) { GetGraphics()->CreateBuffer(*this); }
	void SetData(const void* data, u32 size) { GetGraphics()->SetBufferData(*this, data, size); }
	void SetSubData(const void* data, u32 size, u32 offset) { GetGraphics()->SetBufferSubData(*this, data, size, offset); }
	void SetBindBase(u32 base) { GetGraphics()->SetBufferBindBase(*this, base); }

private:
	Target m_target{};
	u32 m_size{};
};

}

