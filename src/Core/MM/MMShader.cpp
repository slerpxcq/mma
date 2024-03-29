#include "mmpch.hpp"
#include "MMShader.hpp"

#include "Core/GL/GLVertexArray.hpp"

namespace mm
{
	std::shared_ptr<GLVertexAttrib> MMShader::s_vertexAttrib;
	std::shared_ptr<GLVertexAttrib> MMShader::s_morphAttrib;

	void MMShader::VertexAttrib::Set(GLVertexArray& vao) const
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

	void MMShader::MorphAttrib::Set(GLVertexArray& vao) const
	{
		uint32_t vaoid = vao.GetId();
		glVertexArrayAttribFormat(vaoid, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Layout, posOffset));
		glVertexArrayAttribFormat(vaoid, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Layout, uvOffset));
		glVertexArrayAttribIFormat(vaoid, 2, 1, GL_INT, offsetof(Layout, vertexIndex));

		glEnableVertexArrayAttrib(vaoid, 0);
		glEnableVertexArrayAttrib(vaoid, 1);
		glEnableVertexArrayAttrib(vaoid, 2);

		glVertexArrayAttribBinding(vaoid, 0, 0);
		glVertexArrayAttribBinding(vaoid, 1, 0);
		glVertexArrayAttribBinding(vaoid, 2, 0);
	}

	MMShader::MMShader()
	{
		if (s_vertexAttrib == nullptr) 
			s_vertexAttrib = std::make_shared<VertexAttrib>();

		if (s_morphAttrib == nullptr) 
			s_morphAttrib = std::make_shared<MorphAttrib>();
	}

	MMShader::~MMShader()
	{
	}
}