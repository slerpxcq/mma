#include "EditorPch.hpp"
#include "EditorLayer.hpp"

#include "Panel/ViewportPanel.hpp"
#include "Panel/MenuBarPanel.hpp"
#include "Panel/SceneHierarchyPanel.hpp"
#include "Panel/TransformEditorOverlay.hpp"
#include "Panel/CameraControllerOverlay.hpp"
#include "Panel/PhysicsDebugDrawOverlay.hpp"

/* BEGIN TEST INCLUDE */
#include "Core/SceneManager.hpp"
#include "Core/Physics/PhysicsManager.hpp"

#include "Core/Graphics/Shader.hpp"
#include "Core/Graphics/Program.hpp"

#include "Core/File/Text.hpp"
#include "Core/File/PMXFile.hpp"
#include "Core/File/VPDFile.hpp"

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
	auto viewportPanel = MakeScoped<ViewportPanel>("Viewport");
	viewportPanel->SetViewport(GetMainViewport());
	viewportPanel->PushOverlay<TransformEditorOverlay>();
	auto physicsDebugDrawOverlay = viewportPanel->PushOverlay<PhysicsDebugDrawOverlay>();
	viewportPanel->PushOverlay<CameraControllerOverlay>();
	m_panels.push_back(std::move(viewportPanel));
	m_panels.push_back(MakeScoped<MenuBarPanel>());
	m_panels.push_back(MakeScoped<SceneHierarchyPanel>());
	GetPhysicsManager()->SetDebugDraw(physicsDebugDrawOverlay->GetDebugDraw());

	/* BEGIN TEST CODE */
	// Load model
	auto sm = GetSceneManager();
	auto root = sm->GetRootNode();
	auto pmx =  PMXFile("../../resources/model/つみ式ミクさん/000 ミクさん.pmx");
	//auto pmx = PMXFile{ "../../resources/model/つみ式ミクさんv4/つみ式ミクさんv4.pmx"};
	//auto pmx = PMXFile{ "../../resources/model/Tda Maid Miku by iRon0129/Tda Maid Miku 1.00 by iRon0129.pmx" };
	auto modelNode = root->AddChild("model_node");
	auto model = sm->CreateObject<Model>(pmx);
	modelNode->AttachObject(model);
	//auto vpd = VPDFile("../../resources/motion/Standing 1 -Stellas Shoujo Walk.vpd");
	auto vpd = VPDFile("../../resources/motion/test2.vpd");
	model->LoadPose(vpd);
	/* END TEST CODE */
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
