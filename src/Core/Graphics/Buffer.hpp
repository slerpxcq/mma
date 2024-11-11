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
					Graphics::BufferFlags flags = Graphics::BufferFlags::NONE) const;
	void SetSubData(const void* data, u32 size, u32 offset) const;
	void SetBindBase(u32 base) const;
	void* Map(Graphics::BufferAccess access) const;
	void Unmap() const;

private:
	Target m_target{};
	u32 m_size{};
};

}

