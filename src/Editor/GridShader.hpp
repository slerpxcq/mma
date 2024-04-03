#pragma once

#include "Core/GL/GLVertexAttrib.hpp"
#include "Core/GL/GLShader.hpp"

namespace mm
{
	class GLVertexArray;

	class GridShader : public GLShader
	{
	public:
		class Attrib : public GLVertexAttrib {
		public:
			struct Layout {
				glm::vec3 position;
			};
		private:
			void Set(GLVertexArray&) const override;
			uint32_t GetSize() const override {
				return sizeof(Layout);
			}
		};

	public:
		GridShader();
		GLVertexAttrib* GetAttrib() { return m_attrib.get(); }

	private:
		std::unique_ptr<GLVertexAttrib> m_attrib;
	};
}

