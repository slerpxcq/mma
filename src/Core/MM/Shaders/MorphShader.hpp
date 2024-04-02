#pragma once

#include "Core/GL/GLShader.hpp"
#include "Core/GL/GLVertexAttrib.hpp"
#include "Core/GL/GLVertexArray.hpp"

namespace mm
{ 
	class MorphShader : public GLShader
	{
	public:
		static constexpr uint32_t MORPH_BASE = 2;
	public:
		// Morph data layout, std140
		struct Layout {
			glm::vec3 pos;
			float     pad0;
			glm::vec2 uv;
			glm::vec2 pad1;
		}; 

		class Attrib : public GLVertexAttrib {
		public:
			struct Layout {
				glm::vec3 posOffset;
				glm::vec2 uvOffset;
				int32_t vertexIndex;
			};
		private:
			void Set(GLVertexArray&) const override;
			uint32_t GetSize() const override {
				return sizeof(Layout);
			}
		};

	public:
		MorphShader() {
			m_attrib = std::make_unique<Attrib>();
		}

		GLVertexAttrib* GetAttrib() { return m_attrib.get(); }

	private:
		std::unique_ptr<Attrib> m_attrib;
	};
}

