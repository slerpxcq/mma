#pragma once

#include "Core/GL/GLShader.hpp"
#include "Core/GL/GLVertexArray.hpp"
#include "Core/GL/GLBuffer.hpp"

namespace mm
{
	class Viewport;
	class GLRenderer;
	class GridShader;

	class Grid
	{
	public:
		Grid(Viewport&);
		void LoadGrid();
		void Render(GLRenderer&);

	private:
		Viewport& m_viewport;

		GridShader* m_gridShader = nullptr;

		std::unique_ptr<GLVertexArray> m_vertexArray;
		std::unique_ptr<GLBuffer> m_vertexBuffer;
		uint32_t m_vertexCount = 0;
	};
}

