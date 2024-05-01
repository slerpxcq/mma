#include "mmpch.hpp"
#include "SelectionBox.hpp"

#include "Core/Utility/Type.hpp"

namespace mm
{
	void SelectionBox::OnUIRender()
	{
		m_drawList = ImGui::GetWindowDrawList();

		m_using = ImGui::IsMouseDragging(ImGuiMouseButton_Left);
		m_lastFrameUsed = m_using;

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			m_p1 = ImGui::GetMousePos();
			m_min = m_max = m_p2 = m_p1;
		}
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
			m_p2 = ImGui::GetMousePos();
			m_min = ImVec2(std::min(m_p1.x, m_p2.x), std::min(m_p1.y, m_p2.y));
			m_max = ImVec2(std::max(m_p1.x, m_p2.x), std::max(m_p1.y, m_p2.y));
			m_drawList->AddRect(m_min, m_max, FILL_COLOR);
			m_drawList->AddRectFilled(m_min, m_max, OUTLINE_COLOR);
		}
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
		}
	}

	bool SelectionBox::CheckIntersection(const ImVec2& center, float radius)
	{
		if (!ImGui::IsMouseDragging(ImGuiMouseButton_Left))
			return false;

		ImVec2 bbMin = center + ImVec2(radius, radius);
		ImVec2 bbMax = center - ImVec2(radius, radius);
		return (bbMin.x > m_min.x && bbMin.y > m_min.y && bbMax.x < m_max.x && bbMax.y < m_max.y);
	}
}
