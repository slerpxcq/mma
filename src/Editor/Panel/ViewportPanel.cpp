#include "EditorPch.hpp"
#include "ViewportPanel.hpp"

#include "Core/Viewport.hpp"
#include "Core/Node.hpp"
#include "Common/Math/Cast.hpp"

#include <imgui.h>

namespace mm
{

void ViewportPanel::OnUpdate(f32 deltaTime)
{
	if (m_viewport && m_resized) {
		m_viewport->GetFrameBuffer().Resize(m_contentSize.x, m_contentSize.y);
		MM_APP_TRACE("viewport resized; width={0}, height={1}", m_contentSize.x, m_contentSize.y);
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

	if (m_viewport) {
		ImGui::Image(reinterpret_cast<void*>(m_viewport->GetFrameBuffer().GetAttachment(Graphics::AttachmentType::COLOR, 0)->GetID()),
					 ImVec2(m_contentSize.x, m_contentSize.y),
					 ImVec2(0, 1), ImVec2(1, 0));
	} else {
		ImGui::Text("ERROR: no framebuffer is linked");
	}


	Panel::OnEnd();
	ImGui::End();
	ImGui::PopStyleVar();
}

Vec3 ViewportPanel::ToScreenPos(Vec3 worldPos)
{
	Mat4 viewProjection = m_viewport->GetViewProjectionMatrix();
	Vec4 ndcPos = viewProjection * Vec4(worldPos, 1);
	ndcPos /= ndcPos.w;
	return Vec3((ndcPos.x + 1.f) * 0.5f * m_contentSize.x + m_contentPos.x,
				(-ndcPos.y + 1.f) * 0.5f * m_contentSize.y + m_contentPos.y,
				ndcPos.z);
}

}
