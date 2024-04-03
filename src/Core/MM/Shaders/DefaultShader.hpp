#pragma once

#include "Core/GL/GLShader.hpp"
#include "Core/GL/GLBuffer.hpp"
#include "Core/GL/GLVertexAttrib.hpp"

namespace mm
{
	class DefaultShader : public GLShader 
	{
	public:
		static constexpr uint32_t MATERIAL_BASE = 0;
		static constexpr uint32_t SKINNING_BASE = 1;
		static constexpr int32_t ALBEDO_TEX_UNIT = 0;
		static constexpr int32_t SPH_TEX_UNIT = 1;
		static constexpr int32_t TOON_TEX_UNIT = 2;

		static constexpr uint32_t SPH_MODE_OFFSET = 8;
		static constexpr uint32_t TOON_FLAG_OFFSET = 16;

	public:
		// Material layout, std140
		struct MaterialLayout {
			glm::vec4 diffuse;
			glm::vec4 specular;
			glm::vec4 ambient;
			glm::vec4 edge;
			float edgeSize;
			uint32_t flags;
		};

		class Attrib : public GLVertexAttrib {
		public:
			struct Layout {
				glm::vec3 position;
				glm::vec3 normal;
				glm::vec2 uv;
				glm::ivec4 bones;
				glm::vec3 weights;
			};
		private:
			void Set(GLVertexArray&) const override;
			uint32_t GetSize() const override {
				return sizeof(Layout);
			}
		};

	public:
		DefaultShader();
		void SetMaterial(const MaterialLayout& material);
		GLVertexAttrib* GetAttrib() { return m_attrib.get(); }

	private:
		std::unique_ptr<GLVertexAttrib> m_attrib;
		std::unique_ptr<GLBuffer> m_matrialBuffer;
	};
}
