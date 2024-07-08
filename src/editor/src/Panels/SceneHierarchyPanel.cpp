#include "EditorPch.hpp"
#include "SceneHierarchyPanel.hpp"

namespace mm
{

void SceneHierarchyPanel::OnUIRender()
{
	ImGui::Begin("Scene Hierarchy");
	Panel::OnUIRender();

	m_itemID = 0;

	auto scene = m_sceneNode.lock();
	if (scene) {
		Visit(scene);
	} else {
		ImGui::Text("No scene was loaded.");
	}

	ImGui::End();
}

void SceneHierarchyPanel::Visit(std::shared_ptr<Node> node)
{
	ImGui::PushID(m_itemID++);
	bool isOpen = ImGui::TreeNodeEx(node->GetName().c_str(), TREE_FLAGS);
	ImGui::PopID();

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		// Notify item clicked
	}

	if (isOpen) {
		for (auto child : node->GetChildren()) {
			Visit(child);
		}
		ImGui::TreePop();
	}
}

}
