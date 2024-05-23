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

	Visit(&RootNode::Get());

	ImGui::End();
}

void SceneHierarchyPanel::Visit(Node* node)
{
	if (ImGui::TreeNode(node->m_name.c_str())) {
		EventBus::Get().postpone<EditorEvent::NodeSelected>({ node });

		for (auto& child : node->m_children) {
			Visit(child.get());
		}

		ImGui::TreePop();
	}
}

}
