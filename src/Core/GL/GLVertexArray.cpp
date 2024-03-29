#include "mmpch.hpp"
#include "GLVertexArray.hpp"

#include "GLBuffer.hpp"
#include "GLVertexAttrib.hpp"

// 3/26/2024
// https://www.khronos.org/opengl/wiki/Common_Mistakes
// Hidden binding

namespace mm
{
	GLVertexArray::GLVertexArray() :
		m_elemSize(0)
	{
		glCreateVertexArrays(1, &m_id);
		MM_INFO("id={0}: vertex array loaded", m_id);
	}

	void GLVertexArray::Bind()
	{
		glBindVertexArray(m_id);
	}

	void GLVertexArray::Unbind()
	{
		glBindVertexArray(0);
	}

	void GLVertexArray::SetVertexBuffer(const GLBuffer& vbo, uint32_t vertexSize)
	{
		MM_ASSERT(vbo.GetTarget() == GL_ARRAY_BUFFER);
		glVertexArrayVertexBuffer(m_id, 0, vbo.GetId(), 0, vertexSize);
	}

	void GLVertexArray::SetElemBuffer(const GLBuffer& ebo)
	{
		MM_ASSERT(ebo.GetTarget() == GL_ELEMENT_ARRAY_BUFFER);
		glVertexArrayElementBuffer(m_id, ebo.GetId());
	}

	void GLVertexArray::SetVertexAttrib(const GLVertexAttrib& attrib)
	{
		attrib.Set(*this);
	}

	void GLVertexArray::SetElemType(uint32_t type)
	{
		m_elemType = type;
		switch (m_elemType) {
		case GL_UNSIGNED_BYTE:
			m_elemSize = sizeof(uint8_t);
			break;
		case GL_UNSIGNED_SHORT:
			m_elemSize = sizeof(uint16_t);
			break;
		case GL_UNSIGNED_INT:
			m_elemSize = sizeof(uint32_t);
			break;
		}
	}

	void GLVertexArray::DrawArray(uint32_t mode, uint32_t first, uint32_t count)
	{
		glDrawArrays(mode, first, count);
	}

	void GLVertexArray::DrawElem(uint32_t mode, uint32_t first, uint32_t count)
	{
		glDrawElements(mode, count, m_elemType, (void*)(first * m_elemSize));
	}

	GLVertexArray::~GLVertexArray()
	{
		glDeleteVertexArrays(1, &m_id);
		MM_INFO("id={0}: vertex array unloaded", m_id);
	}
}

