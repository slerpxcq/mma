#include "CorePch.hpp"
#include "Buffer.hpp"

namespace mm
{

Buffer::Buffer(Target target) :
	m_target{ target }
{ 
	GetGraphics()->CreateBuffer(*this); 
}

Buffer::~Buffer() 
{ 
	GetGraphics()->DeleteBuffer(*this); 
}

void Buffer::SetSubData(const void* data, u32 size, u32 offset) 
{ 
	GetGraphics()->SetBufferSubData(*this, data, size, offset); 
}

void Buffer::SetBindBase(u32 base) 
{ 
	GetGraphics()->SetBufferBindBase(*this, base); 
}

void Buffer::SetStorage(const void* data, u32 size, Graphics::BufferFlags flags)
{
	GetGraphics()->SetBufferStorage(*this, data, size, flags);
}

void* Buffer::Map(Graphics::BufferAccess access)
{
	return GetGraphics()->MapBuffer(*this, access);
}

void Buffer::Unmap()
{
	GetGraphics()->UnmapBuffer(*this);
}

}

