#include "CorePch.hpp"
#include "Graphics_GL.hpp"

#include "../Buffer.hpp"

#include "glad/glad.h"

namespace mm
{

static u32 ToGLTarget(Buffer::Target target)
{
	switch (target) {
	case Buffer::Target::VERTEX:
		return GL_ARRAY_BUFFER;
	case Buffer::Target::INDEX:
		return GL_ELEMENT_ARRAY_BUFFER;
	default:
		MM_UNINPLEMENTED();
	}
}

void Graphics_GL::CreateBuffer(Buffer& buffer) const
{
	glCreateBuffers(1, buffer.GetIDPtr());
}

void Graphics_GL::DeleteBuffer(Buffer& buffer) const
{
	glDeleteBuffers(1, buffer.GetIDPtr());
}

void Graphics_GL::SetBufferData(Buffer& buffer, void* data, u32 size) const
{
	glNamedBufferData(buffer.GetID(), size, data, GL_STATIC_DRAW);
}

void Graphics_GL::SetBufferSubData(Buffer& buffer, void* data, u32 size, u32 offset) const
{
	glNamedBufferSubData(buffer.GetID(), offset, size, data);
}

void Graphics_GL::SetBufferBindBase(Buffer& buffer, u32 base) const
{
	glBindBufferBase(ToGLTarget(buffer.GetTarget()), base, buffer.GetID());
}

}
