#include "EditorPch.hpp"
#include "SceneHierarchyPanel.hpp"

namespace mm
{

void SceneHierarchyPanel::OnUIRender()
{
	ImGui::Begin("Scene Hierarchy");
	Panel::OnUIRender();

	auto scene = m_sceneNode.lock();
	if (scene) {

	} else {
		ImGui::Text("No scene was loaded.");
	}

	ImGui::End();
}

void SceneHierarchyPanel::Visit(std::shared_ptr<Node> node)
{
	for (auto child : node->GetChildren()) {
		Visit(child);
	}
}

}
