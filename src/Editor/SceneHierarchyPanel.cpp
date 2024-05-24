#include "mmpch.hpp"
#include "SceneHierarchyPanel.hpp"

#include "Core/Nodes/RootNode.hpp"

#include "Core/App/EventBus.hpp"
#include "EditorEvent.hpp"

namespace mm
{

void SceneHierarchyPanel::OnUIRender()
{
	ImGui::Begin("Scene hierarchy");

	m_itemID = 0;
	Visit(&RootNode::Get());

	ImGui::End();
}

void SceneHierarchyPanel::Visit(Node* node)
{
	ImGui::PushID(m_itemID++);
	bool isOpen = ImGui::TreeNodeEx(node->m_name.c_str(), TREE_FLAGS);
	ImGui::PopID();
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		EventBus::Get().postpone<EditorEvent::NodeSelected>({ node });
	}
	if (isOpen) { 
		for (auto& child : node->m_children) {
			Visit(child.get());
		}
		ImGui::TreePop();
	}
}

}
