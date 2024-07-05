#include "EditorPch.hpp"
#include "Panel.hpp"

namespace mm
{

void Panel::OnUIRender()
{
	m_focused = ImGui::IsWindowFocused();
	m_hovered = ImGui::IsWindowHovered();
	m_opened = ImGui::IsWindowAppearing();

	uint32_t width = ImGui::GetWindowWidth();
	uint32_t height = ImGui::GetWindowHeight();

	if (width != m_windowSize.x || height != m_windowSize.y) {
		m_windowSize = { width, height };
		m_contentMin = { ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMin().y };
		m_contentMax = { ImGui::GetWindowContentRegionMax().x, ImGui::GetWindowContentRegionMax().y };
		m_contentSize = m_contentMax - m_contentMin;
		m_contentPos = m_contentMin + glm::uvec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
		m_resized = true;
	} else {
		m_resized = false;
	}
}

}
