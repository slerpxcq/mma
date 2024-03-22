#include "mmpch.hpp"
#include "GLBuffer.hpp"

namespace mm
{
	GLBuffer::GLBuffer(uint32_t target) :
		m_id(0),
		m_target(target)
	{
		glGenBuffers(1, &m_id);
	}

	void GLBuffer::Data(uint32_t size, void* data)
	{ 
		glBindBuffer(m_target, m_id);
		glBufferData(m_target, size, data, GL_STATIC_DRAW);
	}

	void GLBuffer::SubData(uint32_t offset, uint32_t size, void* data)
	{
		glBindBuffer(m_target, m_id);
		glBufferSubData(m_target, offset, size, data);
	}

	void GLBuffer::Base(uint32_t base)
	{
		if (m_target != GL_SHADER_STORAGE_BUFFER) {
			MM_ERROR("Invalid op");
			return;
		}
		glBindBufferBase(m_target, base, m_id);
	}

	GLBuffer::~GLBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}
}


