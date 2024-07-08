#include "EditorPch.hpp"
#include "ViewportPanel.hpp"

#include <renderer/src/GPUResourceManager.hpp>

namespace mm
{

ViewportPanel::ViewportPanel()
{
	std::string countStr = std::to_string(s_count);
	m_name = "Viewport " + countStr;
	m_framebuffer = FrameBuffer::CreateDefault("rt_viewport_" + countStr);
	s_count++;
}

void ViewportPanel::OnUIRender()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin(m_name.c_str(), nullptr, 
				 ImGuiWindowFlags_NoCollapse | 
	//			 ImGuiWindowFlags_NoBringToFrontOnFocus |
				 ImGuiWindowFlags_NoScrollbar |
				 ImGuiWindowFlags_NoScrollWithMouse);

	Panel::OnUIRender();

	if (m_resized) {
		m_framebuffer->Resize(m_windowSize);
		if (auto cam = m_camera.lock(); cam) {
			cam->SetAspect(m_contentSize.x / m_contentSize.y);
		}
	}

	ImGui::Image((void*)m_framebuffer->GetID(),
				 ImVec2(m_contentSize.x, m_contentSize.y),
				 ImVec2(0, 1), ImVec2(1, 0));

	m_framebuffer->Clear(glm::vec4(1, .1, .1, 1), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui::End();
	ImGui::PopStyleVar();
}

}
