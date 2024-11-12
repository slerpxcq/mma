#include "EditorPch.hpp"
#include "Panel.hpp"

#include "Common/Math/Cast.hpp"

#include <imgui.h>

namespace mm
{

void Panel::OnUpdate(f32 deltaTime)
{
	for (auto& overlay : m_overlays)  {
		overlay->OnUpdate(deltaTime);
	}
}

void Panel::OnBegin()
{
	m_focused = ImGui::IsWindowFocused();
	m_hovered = ImGui::IsWindowHovered();
	m_opened = ImGui::IsWindowAppearing();

	Vec2 size = Cast<Vec2>(ImGui::GetWindowSize());
	if (m_windowSize != size) {
		m_windowSize = size;
		m_contentMin = Cast<Vec2>(ImGui::GetWindowContentRegionMin());
		m_contentMax = Cast<Vec2>(ImGui::GetWindowContentRegionMax());
		Vec2 pos = Cast<Vec2>(ImGui::GetWindowPos());
		m_contentSize = m_contentMax - m_contentMin;
		m_contentPos = m_contentMin + pos;
		m_resized = true;
	} else {
		m_resized = false;
	}
}

void Panel::OnEnd()
{
	for (auto& overlay : m_overlays) {
		overlay->OnRender();
	}
}

}
