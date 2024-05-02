#pragma once

#include "Core/GL/GLFrameBuffer.hpp"
#include "Core/GL/GLFrameBufferMS.hpp"

#include "CameraController.hpp"
#include "Grid.hpp"

namespace mm
{
	class Renderer;
	class EditorLayer;

	class Viewport
	{
	public:
		Viewport(EditorLayer& editor);
		void OnUpdate(float deltaTime);
		void OnRender(Renderer& renderer);
		void OnUIRender();
		ImVec2 GetSize() const { return m_size; }
		bool IsHovered() const { return m_hovered; }

	private:
		EditorLayer& m_editor;
		CameraController m_cameraController;

		std::unique_ptr<Grid> m_grid;
		std::unique_ptr<GLFrameBuffer> m_framebuffer;

		std::unique_ptr<GLFrameBufferMS> m_framebufferMS;

		ImVec2 m_size;

		bool m_hovered;
	};
}


