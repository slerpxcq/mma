#include "mmpch.hpp"
#include "Grid.hpp"

#include "Core/App/Application.hpp"

#include "Viewport.hpp"

#include "Core/GL/GLRenderer.hpp"

#include "GridShader.hpp"

namespace mm
{
	Grid::Grid(Viewport& viewport) :
		m_viewport(viewport)
	{

		auto gridShader = std::make_unique<GridShader>();
		gridShader->Compile("resources/shaders/grid.vert", GLShader::VERTEX);
		gridShader->Compile("resources/shaders/grid.frag", GLShader::FRAGMENT);
		gridShader->Link();

		m_gridShader = gridShader.get();

		ResourceManager::s_instance.LoadShader("grid", std::move(gridShader));

		LoadGrid();
	}

	void Grid::LoadGrid()
	{
		std::vector<GridShader::Attrib::Layout> vertices;
		for (int32_t i = 0; i <= 16; ++i) {
			vertices.push_back({ glm::vec3(i - 8, 0, -8) });
			vertices.push_back({ glm::vec3(i - 8, 0, 8) });
		}
		for (int32_t i = 0; i <= 16; ++i) {
			vertices.push_back({ glm::vec3(-8, 0, i - 8) });
			vertices.push_back({ glm::vec3(8, 0, i - 8) });
		}

		m_vertexBuffer = std::make_unique<GLBuffer>(GL_ARRAY_BUFFER);
		m_vertexBuffer->SetData(vertices.size() * sizeof(vertices[0]), vertices.data());
		m_vertexArray = std::make_unique<GLVertexArray>();
		m_vertexArray->SetVertexBuffer(*m_vertexBuffer, sizeof(vertices[0]));
		m_vertexArray->SetVertexAttrib(*m_gridShader->GetAttrib());
		m_vertexCount = vertices.size();
	}

	void Grid::Render(GLRenderer& renderer)
	{
		renderer.BeginShader(m_gridShader);
		static glm::vec4 color(1, 1, 1, 0.5);
		renderer.Uniform("u_color", 1, &color);
		m_vertexArray->Bind();
		m_vertexArray->DrawArray(GL_LINES, 0, m_vertexCount);
		renderer.EndShader();
	}
}
