#pragma once

#include "Core/GL/GLFrameBuffer.hpp"

#include "CameraController.hpp"
#include "Grid.hpp"

namespace mm
{
	class GLRenderer;
	class EditorLayer;

	class Viewport
	{
	public:
		Viewport(EditorLayer& editor);
		void OnUpdate(float deltaTime);
		void OnRender(GLRenderer& renderer);
		void OnUIRender();
		glm::uvec2 GetSize() const { return m_size; }
		glm::ivec2 GetPos() const { return m_pos; }

	private:
		EditorLayer& m_editor;
		CameraController m_cameraController;
		std::unique_ptr<GLFrameBuffer> m_framebuffer;
		glm::uvec2 m_size;
		glm::ivec2 m_pos;

		std::unique_ptr<Grid> m_grid;

		bool m_focused;
	};
}


