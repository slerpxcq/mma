#include "EditorPch.hpp"
#include "EditorLayer.hpp"

#include "Panel/ViewportPanel.hpp"
#include "Panel/MenuBarPanel.hpp"
#include "Panel/SceneHierarchyPanel.hpp"

/* BEGIN TEST INCLUDE */
#include "Core/SceneManager.hpp"

#include "Core/Graphics/Shader.hpp"
#include "Core/Graphics/Program.hpp"

#include "Core/File/Text.hpp"
#include "Core/File/PMXFile.hpp"

#include "Core/Node.hpp"
#include "Core/Model.hpp"

#include "Core/Viewport.hpp"
/* END TEST INCLUDE */

#include <imgui.h>

namespace mm
{

EditorLayer::EditorLayer(const Window& window) :
	ImGuiLayer{ window }
{
	auto vp = MakeScoped<ViewportPanel>("Viewport");
	vp->SetViewport(GetMainViewport());
	m_panels.push_back(std::move(vp));
	m_panels.push_back(MakeScoped<MenuBarPanel>());
	m_panels.push_back(MakeScoped<SceneHierarchyPanel>());

	/* BEGIN TEST CODE */
	// Load model
	auto& root = GetSceneManager()->GetRootNode();
	auto pmx =  PMXFile::Load("../../resources/model/つみ式ミクさん/000 ミクさん.pmx");
	auto& modelNode = root.AddChild("model_node");
	auto model = Model::Load(modelNode, *pmx);
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
