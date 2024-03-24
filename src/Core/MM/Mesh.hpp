#pragma once

#include "Core/GL/GLBuffer.hpp"
#include "Core/GL/GLShader.hpp"
#include "Core/GL/GLVertexArray.hpp"
#include "Core/GL/GLTexture.hpp"

namespace mm
{
	struct Material
	{
		glm::vec4 diffuse;
		glm::vec4 specular;
		glm::vec4 ambient;
		glm::vec4 edge;
		float edgeSize;
		uint32_t flags;
		int32_t albedoIndex;
		int32_t sphIndex;
		int32_t toonIndex;
		GLShader* shader;
	};

	struct Mesh
	{
		GLVertexArray vertexArray;
		Material material;
		uint32_t faceCount;
		std::unique_ptr<GLBuffer> elemBuf;
	};
}

