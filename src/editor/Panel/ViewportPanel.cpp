#include "EditorPch.hpp"
#include "ViewportPanel.hpp"

#include <imgui.h>

namespace mm
{

void ViewportPanel::OnUpdate(f32 deltaTime)
{
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
		m_frameBuffer->ClearColor(0, { 1.f, 0.f, 0.f, 1.f });
		ImGui::Image((void*)m_frameBuffer->GetID(),
					 ImVec2(m_contentSize.x, m_contentSize.y),
					 ImVec2(0, 1), ImVec2(1, 0));
	} else {
		ImGui::Text("ERROR: no framebuffer is linked");
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

}
