#include "mmpch.hpp"
#include "SkinVertex.hpp"

#include "Core/GL/GLVertexArray.hpp"

namespace mm
{
	SkinVertex SkinVertex::s_instance;

	void SkinVertex::Set(GLVertexArray& vao) const
	{
		uint32_t vaoid = vao.GetId();
		glVertexArrayAttribFormat(vaoid, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Layout, position));
		glVertexArrayAttribFormat(vaoid, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Layout, normal));
		glVertexArrayAttribFormat(vaoid, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Layout, uv));
		glVertexArrayAttribIFormat(vaoid, 3, 4, GL_INT, offsetof(Layout, bones));
		glVertexArrayAttribFormat(vaoid, 4, 3, GL_FLOAT, GL_FALSE, offsetof(Layout, weights));
		glVertexArrayAttribFormat(vaoid, 5, 3, GL_FLOAT, GL_FALSE, offsetof(Layout, sdef_c));
		glVertexArrayAttribFormat(vaoid, 6, 3, GL_FLOAT, GL_FALSE, offsetof(Layout, sdef_r0));
		glVertexArrayAttribFormat(vaoid, 7, 3, GL_FLOAT, GL_FALSE, offsetof(Layout, sdef_r1));

		glEnableVertexArrayAttrib(vaoid, 0);
		glEnableVertexArrayAttrib(vaoid, 1);
		glEnableVertexArrayAttrib(vaoid, 2);
		glEnableVertexArrayAttrib(vaoid, 3);
		glEnableVertexArrayAttrib(vaoid, 4);
		glEnableVertexArrayAttrib(vaoid, 5);
		glEnableVertexArrayAttrib(vaoid, 6);
		glEnableVertexArrayAttrib(vaoid, 7);

		glVertexArrayAttribBinding(vaoid, 0, 0);
		glVertexArrayAttribBinding(vaoid, 1, 0);
		glVertexArrayAttribBinding(vaoid, 2, 0);
		glVertexArrayAttribBinding(vaoid, 3, 0);
		glVertexArrayAttribBinding(vaoid, 4, 0);
		glVertexArrayAttribBinding(vaoid, 5, 0);
		glVertexArrayAttribBinding(vaoid, 6, 0);
		glVertexArrayAttribBinding(vaoid, 7, 0);
	}
}
