#pragma once

namespace mm
{

class Node;

class SceneHierarchyPanel
{
	static constexpr uint32_t TREE_FLAGS = ImGuiTreeNodeFlags_OpenOnArrow;
public:
	void OnUIRender();

private:
	void Visit(Node* node);

private:
	uint32_t m_itemID = 0;
};

}

