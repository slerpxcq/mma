#pragma once

#include "Core/GL/GLVertexAttrib.hpp"

namespace mm
{
	class SkinVertex : public GLVertexAttrib
	{
	public:
		struct Layout {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 uv;
			glm::ivec4 bones;
			glm::vec3 weights;
			glm::vec3 sdef_c;
			glm::vec3 sdef_r0;
			glm::vec3 sdef_r1;
		};

	public:
		static SkinVertex s_instance;

	private:
		void Set(GLVertexArray&) const override;
	};
}

