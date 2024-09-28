#include "CorePch.hpp"
#include "Buffer_GL.hpp"

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
	}
}

Buffer_GL::Buffer_GL(Target target) :
	Buffer{target}
{
	glGenBuffers(1, &m_id);
}

Buffer_GL::~Buffer_GL()
{
	glDeleteBuffers(1, &m_id);
}
void Buffer_GL::SetData(void* data, u32 size)
{
	glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
}

void Buffer_GL::SetSubData(void* data, u32 size, u32 offset)
{
	glNamedBufferSubData(ToGLTarget(m_target), offset, size, data);
}

void Buffer_GL::SetBindBase(u32 base)
{
	glBindBufferBase(ToGLTarget(m_target), base, m_id);
}

}
