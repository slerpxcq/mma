#include "CorePch.hpp"
#include "Graphics_GL.hpp"

#include "../Buffer.hpp"
#include "../VertexArray.hpp"

#include <glad/glad.h>

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
	MM_CORE_INFO("GL: buffer created; id={0}", buffer.GetID());
}

void Graphics_GL::DeleteBuffer(Buffer& buffer) const
{
	glDeleteBuffers(1, buffer.GetIDPtr());
	MM_CORE_INFO("GL: buffer deleted; id={0}", buffer.GetID());
}

void Graphics_GL::SetBufferData(const Buffer& buffer, const void* data, u32 size) const
{
	glNamedBufferData(buffer.GetID(), size, data, GL_STATIC_DRAW);
}

void Graphics_GL::SetBufferSubData(const Buffer& buffer, const void* data, u32 size, u32 offset) const
{
	glNamedBufferSubData(buffer.GetID(), offset, size, data);
}

void Graphics_GL::SetBufferBindBase(const Buffer& buffer, u32 base) const
{
	glBindBufferBase(ToGLTarget(buffer.GetTarget()), base, buffer.GetID());
}

void Graphics_GL::CreateVertexArray(VertexArray& va) const
{
	glCreateVertexArrays(1, va.GetIDPtr());
	MM_CORE_INFO("GL: vertex array created; id={0}", va.GetID());
}

void Graphics_GL::DeleteVertexArray(VertexArray& va) const
{
	glDeleteVertexArrays(1, va.GetIDPtr());
	MM_CORE_INFO("GL: vertex array deleted; id={0}", va.GetID());
}

void Graphics_GL::SetIndexBuffer(const VertexArray& va, const IndexBuffer& ib) const
{
	glVertexArrayElementBuffer(va.GetID(), ib.GetID());
}

void Graphics_GL::SetVertexBuffer(const VertexArray& va, const VertexBuffer& vb) const
{
	// TODO: offset, binding point
	glVertexArrayVertexBuffer(va.GetID(), 0, vb.GetID(), 0, vb.GetLayout().GetSize());
}

void Graphics_GL::SetVertexAttribFormat(const VertexArray& va, u32 location, AttributeType type, u32 count, u32 offset, bool normalized) const
{
	glEnableVertexArrayAttrib(va.GetID(), location);

	switch (type) {
	case AttributeType::INT:
		glVertexArrayAttribIFormat(va.GetID(), location, count, GL_INT, offset);
		break;
	case AttributeType::FLOAT:
		glVertexArrayAttribFormat(va.GetID(), location, count, GL_FLOAT, normalized, offset);
		break;
	}

	glVertexArrayAttribBinding(va.GetID(), location, 0);
}

}
