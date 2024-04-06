#pragma once

#include "Core/GL/GLVertexAttrib.hpp"

namespace mm
{
	class MorphVertex : public GLVertexAttrib
	{
	public:
		struct Layout {
			glm::vec3 posOffset;
			glm::vec2 uvOffset;
			int32_t vertexIndex;
		};

	public:
		static MorphVertex s_instance;

	private:
		void Set(GLVertexArray&) const override;
	};
}

