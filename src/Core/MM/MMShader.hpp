#pragma once

#include "Core/GL/GLShader.hpp"
#include "Core/GL/GLBuffer.hpp"
#include "Core/GL/GLVertexAttrib.hpp"

namespace mm
{
	class MMShader : public GLShader 
	{
	public:
		static constexpr uint32_t MATERIAL_BASE = 0;
		static constexpr uint32_t SKINNING_BASE = 1;
		static constexpr uint32_t MORPH_BASE = 2;
		static constexpr int32_t ALBEDO_TEX_UNIT = 0;
		static constexpr int32_t SPH_TEX_UNIT = 1;
		static constexpr int32_t TOON_TEX_UNIT = 2;

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

		// Morph data layout, std140
		struct MorphLayout {
			glm::vec3 pos;
			float     pad0;
			glm::vec2 uv;
			glm::vec2 pad1;
		}; 

		class VertexAttrib : public GLVertexAttrib {
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

		class MorphAttrib : public GLVertexAttrib {
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
		MMShader();
		~MMShader();
		void SetMaterial(const MaterialLayout& material);

	public:
		static std::shared_ptr<GLVertexAttrib> s_vertexAttrib;
		static std::shared_ptr<GLVertexAttrib> s_morphAttrib;

	private:
		static std::shared_ptr<GLBuffer> s_matrialBuffer;
	};
}
