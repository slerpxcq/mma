#include "EditorPch.hpp"
#include "ViewportPanel.hpp"

#include <imgui.h>

namespace mm
{

void ViewportPanel::OnUpdate(f32 deltaTime)
{
	if (m_frameBuffer && m_resized) {
		m_frameBuffer->Resize(m_contentSize.x, m_contentSize.y);
		MM_APP_INFO("viewport resized; width={0}, height={1}", m_contentSize.x, m_contentSize.y);
	}
}

void ViewportPanel::OnRender()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin(m_name.c_str(), nullptr, 
				 ImGuiWindowFlags_NoCollapse | 
				 ImGuiWindowFlags_NoScrollbar |
				 ImGuiWindowFlags_NoScrollWithMouse);

	Panel::OnBegin();

	if (m_frameBuffer) {
		ImGui::Image(reinterpret_cast<void*>(m_frameBuffer->GetID()),
					 ImVec2(m_contentSize.x, m_contentSize.y),
					 ImVec2(0, 1), ImVec2(1, 0));
	} else {
		ImGui::Text("ERROR: no framebuffer is linked");
	}

	ImGui::End();
	ImGui::PopStyleVar();
} 

}
