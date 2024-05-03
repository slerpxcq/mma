#pragma once

#include "Core/GL/GLBuffer.hpp"
#include "Core/GL/GLVertexAttrib.hpp"
#include "Core/GL/GLShader.hpp"
#include "Core/GL/GLTexture.hpp"
#include "Core/GL/GLTexture2D.hpp"
#include "Core/GL/GLVertexArray.hpp"
#include "Core/GL/GLFrameBuffer.hpp"
#include "Core/MM/Renderer/Renderer.hpp"

#include "Core/MM/Renderer/Effect.hpp"

namespace mm
{
	class Model;
	class Renderer;

	class Skin
	{
	public:
		static constexpr uint32_t TOON_FLAG_BIT = (1 << 16);

	public:
		struct Mesh {
			std::string name;
			MaterialLayout material;
			Effect* effect;
			uint32_t elemCount;
			uint32_t elemOffset;
			int32_t albedoIndex;
			int32_t sphIndex;
			int32_t toonIndex;
		};

	public:
		Skin(Model &model);
		void Render(Renderer& renderer);
		uint32_t GetVertexCount() const { return m_vertexCount;  }
		const std::vector<Mesh>& GetMeshes() const { return m_meshes; }
		std::vector<Mesh>& GetMeshes() { return m_meshes; }
		const GLVertexArray& GetVertexArray() const { return *m_vertexArray; }
		GLTexture& GetTexture(int32_t idx);
		GLTexture& GetToon(const Mesh& mesh);

	private:
		void LoadVertices();
		void LoadIndices();
		void LoadMeshes();
		void LoadTextures();

	private:
		Model& m_model;
		uint32_t m_vertexCount;

		std::vector<Mesh> m_meshes;
		std::vector<std::unique_ptr<GLTexture>> m_textures;

		std::unique_ptr<GLVertexArray> m_vertexArray;
		std::unique_ptr<GLBuffer> m_vertexBuffer;
		std::unique_ptr<GLBuffer> m_elemBuffer;

		Effect& m_defaultEffect;
	};
}


