#pragma once

#include "Core/Camera/Camera.hpp"
#include "Core/GL/GLFrameBuffer.hpp"

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
		const Camera& GetCamera() const { return m_camera; }

	private:
		EditorLayer& m_editor;
		Camera m_camera;
		std::unique_ptr<GLFrameBuffer> m_framebuffer;
		glm::uvec2 m_size;
		glm::ivec2 m_pos;

		bool m_focused;
	};
}


