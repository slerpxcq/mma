#include "mmpch.hpp"
#include "GridShader.hpp"

#include "Core/GL/GLVertexArray.hpp"

namespace mm
{
	void GridShader::Attrib::Set(GLVertexArray& vao) const
	{
		uint32_t vaoid = vao.GetId();
		glVertexArrayAttribFormat(vaoid, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Layout, position));
		glEnableVertexArrayAttrib(vaoid, 0);
		glVertexArrayAttribBinding(vaoid, 0, 0);
	}

	GridShader::GridShader()
	{
		m_attrib = std::make_unique<Attrib>();
	}
}
