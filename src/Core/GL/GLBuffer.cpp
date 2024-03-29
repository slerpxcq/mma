#include "mmpch.hpp"
#include "GLBuffer.hpp"
#include "GLVertexAttrib.hpp"

namespace mm
{
	GLBuffer::GLBuffer(uint32_t target) :
		m_id(0),
		m_target(target),
		m_size(0)
	{
		glCreateBuffers(1, &m_id);
	}

	//void GLBuffer::Bind()
	//{
	//	glBindBuffer(m_target, m_id);
	//}

	void GLBuffer::SetData(uint32_t size, void* data)
	{ 
		m_size = size;
		glNamedBufferData(m_id, size, data, GL_STATIC_DRAW);
		MM_INFO("id={0}: buffer loaded", m_id);
	}

	void GLBuffer::SetSubData(uint32_t offset, uint32_t size, void* data)
	{
		m_size = size;
		glNamedBufferSubData(m_id, offset, size, data);
	}

	void GLBuffer::SetBase(uint32_t base)
	{
		MM_ASSERT(m_target == GL_SHADER_STORAGE_BUFFER || m_target == GL_UNIFORM_BUFFER);
		glBindBufferBase(m_target, base, m_id);
	}

	GLBuffer::~GLBuffer()
	{
		glDeleteBuffers(1, &m_id);
		MM_INFO("id={0}: buffer unloaded", m_id);
	}
}


