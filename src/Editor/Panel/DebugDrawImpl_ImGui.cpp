#include "EditorPch.hpp"
#include "DebugDrawImpl_ImGui.hpp"

#include "PhysicsDebugDrawOverlay.hpp"
#include "ViewportPanel.hpp"

#include <imgui.h>

namespace mm
{

void DebugDrawImpl_ImGui::DrawLine(const Vec3& from, const Vec3& to, const Color& color) const
{
	auto panel = static_cast<ViewportPanel*>(m_parent.GetParent());
	auto p1 = Cast<ImVec2>(Vec2{ panel->ToScreenPos(from) });
	auto p2 = Cast<ImVec2>(Vec2{ panel->ToScreenPos(to) });
	auto drawList = ImGui::GetWindowDrawList();
	auto col = ImGui::ColorConvertFloat4ToU32(Cast<ImVec4>(color));
	drawList->AddLine(p1, p2, col, m_lineSize);
}

}
