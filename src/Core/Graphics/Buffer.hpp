#pragma once

#include "GPUResource.hpp"
#include "Graphics.hpp"

namespace mm
{

class Buffer : public GPUResource
{
public:
	enum class Target {
		VERTEX, INDEX, UNIFORM, SHADER_STORAGE
	};

public:
	virtual ~Buffer();
	Target GetTarget() const { return m_target; }

protected:
	Buffer(Target target);
	void SetStorage(const void* data, u32 size, 
					Graphics::BufferFlags flags = Graphics::BufferFlags::NONE);
	void SetSubData(const void* data, u32 size, u32 offset);
	void SetBindBase(u32 base);
	void* Map(Graphics::BufferAccess access);
	void Unmap();

private:
	Target m_target{};
	u32 m_size{};
};

}

