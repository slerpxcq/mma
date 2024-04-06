#include "mmpch.hpp"
#include "MorphVertex.hpp"

#include "Core/GL/GLVertexArray.hpp"

namespace mm
{
	MorphVertex MorphVertex::s_instance;

	void MorphVertex::Set(GLVertexArray& vao) const
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
}
