#pragma once

#include "Core/GL/GLVertexArray.hpp"
#include "Core/GL/GLBuffer.hpp"
#include "Core/GL/GLVertexAttrib.hpp"

namespace mm
{
	class Viewport;
	class GLRenderer;
	class GridShader;

	class GridVertex : public GLVertexAttrib {
	public:
		struct Layout {
			glm::vec3 position;
		};
	public:
		static GridVertex s_instance;

	private:
		void Set(GLVertexArray&) const override;
	};

	class Grid
	{
		static constexpr int32_t SIZE = 16;
	public:
		Grid(Viewport&);
		void LoadGrid();
		void Render(GLRenderer&);

	private:
		Viewport& m_viewport;

		std::unique_ptr<GLVertexArray> m_vertexArray;
		std::unique_ptr<GLBuffer> m_vertexBuffer;
		uint32_t m_vertexCount = 0;
	};
}

