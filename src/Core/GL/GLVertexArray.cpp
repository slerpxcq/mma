#include "mmpch.hpp"
#include "GLVertexArray.hpp"

#include "GLBuffer.hpp"
#include "GLVertexAttrib.hpp"

namespace mm
{
	GLVertexArray::GLVertexArray() 
	{
		glGenVertexArrays(1, &m_id);
		glBindVertexArray(m_id);
	}

	void GLVertexArray::Bind()
	{
		glBindVertexArray(m_id);
	}

	void GLVertexArray::Attrib(GLVertexAttrib* attrib)
	{
		attrib->Set();
	}

	void GLVertexArray::DrawArray(uint32_t mode, uint32_t first, uint32_t count)
	{
		glBindVertexArray(m_id);
		glDrawArrays(mode, first, count);
	}

	void GLVertexArray::DrawElem(uint32_t mode, uint32_t first, uint32_t count)
	{
		glBindVertexArray(m_id);
		glDrawElements(mode, count, GL_UNSIGNED_INT, (void*)first);
	}

	void GLVertexArray::BindBuffer(const GLBuffer& buffer)
	{
		glBindBuffer(buffer.m_target, buffer.m_id);
	}

	GLVertexArray::~GLVertexArray()
	{
		glDeleteVertexArrays(1, &m_id);
	}
}

