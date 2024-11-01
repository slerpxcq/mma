#include "EditorPch.hpp"
#include "EditorLayer.hpp"

#include "Panel/ViewportPanel.hpp"

/* BEGIN TEST INCLUDE */
#include "Core/Graphics/Shader.hpp"
#include "Core/Graphics/Program.hpp"
#include "Core/File/Text.hpp"
#include "Core/File/PMXFile.hpp"
/* END TEST INCLUDE */

#include <imgui.h>

namespace mm
{

EditorLayer::EditorLayer(const Window& window) :
	ImGuiLayer{ window }
{
	auto vp = MakeScoped<ViewportPanel>("Viewport");
	vp->SetFrameBuffer(GetMainFrameBuffer());
	m_panels.push_back(std::move(vp));

	/* BEGIN TEST CODE */
	auto root = GetRootNode();
	auto pmx =  PMXFile::Load("../../resources/model/つみ式ミクさん/000 ミクさん.pmx");
	root->AttachChild(ModelNode::LoadFromPMX(*pmx));
	/* END TEST CODE */
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnUpdate(f32 deltaTime)
{
	for (auto& panel : m_panels) {
		panel->OnUpdate(deltaTime);
	}
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
