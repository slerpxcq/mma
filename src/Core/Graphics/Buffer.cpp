#include "CorePch.hpp"
#include "Buffer.hpp"

namespace mm
{

Buffer::Buffer(Target target) 
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

void Buffer::SetStorage(const void* data, u32 size, u32 flags)
{
	GetGraphics()->SetBufferStorage(*this, data, size, flags);
}

}

