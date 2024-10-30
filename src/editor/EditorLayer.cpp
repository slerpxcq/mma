#include "EditorPch.hpp"
#include "EditorLayer.hpp"

#include <imgui.h>

namespace mm
{

void EditorLayer::OnUpdate(f32 deltaTime)
{
	for (auto& panel : m_panels) {
		panel->OnUpdate(deltaTime);
	}
}

void EditorLayer::OnRender()
{
	for (auto& panel : m_panels) {
		panel->OnRender();
	}

	/* BEGIN TEST CODE */

	Begin();
	ImGui::ShowDemoWindow();
	End();

	/* END TEST CODE */
}

}
