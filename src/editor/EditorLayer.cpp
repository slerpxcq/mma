#include "EditorPch.hpp"
#include "EditorLayer.hpp"

#include "Panel/ViewportPanel.hpp"

#include <imgui.h>

namespace mm
{

void EditorLayer::OnUpdate(f32 deltaTime)
{
	for (auto& panel : m_panels) {
		panel->OnUpdate(deltaTime);
	}
}

EditorLayer::EditorLayer(const Window& window) :
	ImGuiLayer{ window }
{
	auto vp = MakeScoped<ViewportPanel>("Viewport");
	vp->SetFrameBuffer(GetMainFrameBuffer());
	m_panels.push_back(std::move(vp));
}

void EditorLayer::OnRender()
{
	Begin();

	for (auto& panel : m_panels) {
		panel->OnRender();
	}

	/* BEGIN TEST CODE */
	/* END TEST CODE */

	End();
}

}
