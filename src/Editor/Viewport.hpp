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
		ImVec2 GetSize() const { return m_size; }
		bool IsHovered() const { return m_hovered; }

	private:
		EditorLayer& m_editor;
		CameraController m_cameraController;

		std::unique_ptr<Grid> m_grid;
		std::unique_ptr<GLFrameBuffer> m_framebuffer;

		ImVec2 m_size;

		bool m_hovered;
	};
}


