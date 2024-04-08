#include "mmpch.hpp"
#include "Grid.hpp"

#include "Core/App/Application.hpp"

#include "Viewport.hpp"

#include "Core/GL/GLRenderer.hpp"

namespace mm
{
	GridVertex GridVertex::s_instance;

	void GridVertex::Set(GLVertexArray& vao) const
	{
		uint32_t vaoid = vao.GetId();
		glVertexArrayAttribFormat(vaoid, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Layout, position));
		glEnableVertexArrayAttrib(vaoid, 0);
		glVertexArrayAttribBinding(vaoid, 0, 0);
	}

	Grid::Grid(Viewport& viewport) :
		m_viewport(viewport)
	{
		LoadGrid();
	}

	void Grid::LoadGrid()
	{
		std::vector<GridVertex::Layout> vertices;
		for (int32_t i = 0; i <= SIZE; ++i) {
			vertices.push_back({ glm::vec3(i - SIZE/2, 0, -SIZE/2) });
			vertices.push_back({ glm::vec3(i - SIZE/2, 0, SIZE/2) });
		}
		for (int32_t i = 0; i <= SIZE; ++i) {
			vertices.push_back({ glm::vec3(-SIZE/2, 0, i - SIZE/2) });
			vertices.push_back({ glm::vec3(SIZE/2, 0, i - SIZE/2) });
		}

		m_vertexBuffer = std::make_unique<GLBuffer>(GL_ARRAY_BUFFER);
		m_vertexBuffer->SetData(vertices.size() * sizeof(vertices[0]), vertices.data());
		m_vertexArray = std::make_unique<GLVertexArray>();
		m_vertexArray->SetVertexBuffer(*m_vertexBuffer, sizeof(vertices[0]));
		m_vertexArray->SetVertexAttrib(GridVertex::s_instance);
		m_vertexCount = vertices.size();
	}

	void Grid::Render(GLRenderer& renderer)
	{
		renderer.SetShader(ResourceManager::s_instance.GetShader("grid"));
		renderer.GetShader()->Uniform("u_color", glm::vec4(1, 1, 1, .5));
		m_vertexArray->Bind();
		m_vertexArray->DrawArray(GL_LINES, 0, m_vertexCount);
	}
}
