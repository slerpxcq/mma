#pragma once

#include "Panel.hpp"

#include <core/src/SceneNode.hpp>

namespace mm
{

class SceneHierarchyPanel : public Panel
{
	static constexpr uint32_t TREE_FLAGS = ImGuiTreeNodeFlags_OpenOnArrow;
public:
	virtual void OnUIRender() override;
	void SetScene(std::shared_ptr<SceneNode> node) {
		m_sceneNode = node;
	}

private:
	void Visit(std::shared_ptr<Node> node);

private:
	std::weak_ptr<SceneNode> m_sceneNode;
	uint32_t m_itemID;
};

}

