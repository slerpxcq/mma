#include "EditorPch.hpp"
#include "SceneHierarchyPanel.hpp"

#include "Core/SceneManager.hpp"

#include <imgui.h>

namespace mm
{

void SceneHierarchyPanel::OnUpdate(f32 deltaTime)
{
}

void SceneHierarchyPanel::OnRender()
{
	auto& root = GetSceneManager()->GetRootNode();
	ImGui::Begin(m_name.c_str());
	Panel::OnBegin();
	m_itemID = 0;
	Visit(root);
	ImGui::End();
}

void SceneHierarchyPanel::Visit(Node& node)
{
	static constexpr u32 TREE_FLAGS = ImGuiTreeNodeFlags_OpenOnArrow;

	ImGui::PushID(m_itemID++);
	bool isOpen = ImGui::TreeNodeEx(node.GetName().data(), TREE_FLAGS);
	ImGui::PopID();

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
	}

	if (isOpen) {
		for (auto& child : node.GetChildren()) {
			Visit(*child);
		}
		ImGui::TreePop();
	}
}

}
