#pragma once

#include "Mesh.hpp"
#include "Core/GL/GLBuffer.hpp"
#include "Core/GL/GLVertexAttrib.hpp"

#include <glm/glm.hpp>

#include <vector>

namespace mm
{
	class Model;

	class Skin
	{
	public:
		class Attrib : public GLVertexAttrib {
		public:
			struct Layout {
				glm::vec3 position;
				glm::vec3 normal;
				glm::vec2 uv;
				glm::ivec4 bones;
				glm::vec3 weights;
			};

			void Set() const override;
		};

	public:
		Skin(Model &model);

	private:
		void LoadVertices();
		void LoadMeshes();
		void LoadTextures();

	private:
		Model& m_model;
		std::unique_ptr<Attrib> m_vertexAttrib;

		// position, normal, texcoord
		std::unique_ptr<GLBuffer> m_vbo;

		// Matrial, indices
		std::vector<Mesh> m_meshes;

		std::vector<GLTexture> m_textures;
	};
}

