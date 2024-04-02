#include "mmpch.hpp"
#include "DefaultShader.hpp"

#include "Core/GL/GLVertexArray.hpp"
#include "Core/GL/GLBuffer.hpp"

namespace mm
{
	void DefaultShader::Attrib::Set(GLVertexArray& vao) const
	{
		uint32_t vaoid = vao.GetId();
		glVertexArrayAttribFormat(vaoid, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Layout, position));
		glVertexArrayAttribFormat(vaoid, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Layout, normal));
		glVertexArrayAttribFormat(vaoid, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Layout, uv));
		glVertexArrayAttribIFormat(vaoid, 3, 4, GL_INT, offsetof(Layout, bones));
		glVertexArrayAttribFormat(vaoid, 4, 3, GL_FLOAT, GL_FALSE, offsetof(Layout, weights));

		glEnableVertexArrayAttrib(vaoid, 0);
		glEnableVertexArrayAttrib(vaoid, 1);
		glEnableVertexArrayAttrib(vaoid, 2);
		glEnableVertexArrayAttrib(vaoid, 3);
		glEnableVertexArrayAttrib(vaoid, 4);

		glVertexArrayAttribBinding(vaoid, 0, 0);
		glVertexArrayAttribBinding(vaoid, 1, 0);
		glVertexArrayAttribBinding(vaoid, 2, 0);
		glVertexArrayAttribBinding(vaoid, 3, 0);
		glVertexArrayAttribBinding(vaoid, 4, 0);
	}

	DefaultShader::DefaultShader()
	{
		m_attrib = std::make_unique<Attrib>();

		m_matrialBuffer = std::make_unique<GLBuffer>(GL_UNIFORM_BUFFER);
		m_matrialBuffer->SetBase(MATERIAL_BASE);
		m_matrialBuffer->SetData(sizeof(MaterialLayout), nullptr);
	}

	void DefaultShader::SetMaterial(const MaterialLayout& material)
	{
		m_matrialBuffer->SetSubData(0, sizeof(MaterialLayout), (void*)&material);
	}
}