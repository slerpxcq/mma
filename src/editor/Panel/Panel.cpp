#include "EditorPch.hpp"
#include "Panel.hpp"

#include <imgui.h>

namespace mm
{

void Panel::OnRender()
{
	m_focused = ImGui::IsWindowFocused();
	m_hovered = ImGui::IsWindowHovered();
	m_opened = ImGui::IsWindowAppearing();

	u32 width = ImGui::GetWindowWidth();
	u32 height = ImGui::GetWindowHeight();

	if (width != m_windowSize.x || height != m_windowSize.y) {
		m_windowSize = { width, height };
		auto min = ImGui::GetWindowContentRegionMin();
		auto max = ImGui::GetWindowContentRegionMax();
		auto pos = ImGui::GetWindowPos();
		m_contentMin = { min.x, min.y };
		m_contentMax = { max.x, max.y };
		m_contentSize = m_contentMax - m_contentMin;
		m_contentPos = m_contentMin + Vec2u(pos.x, pos.y);
		m_resized = true;
	} else {
		m_resized = false;
	}
}

}
