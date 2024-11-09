#pragma once

#include "Panel.hpp"

namespace mm
{

class Node;

class SceneHierarchyPanel : public Panel
{
public:
	SceneHierarchyPanel() : 
		Panel{ "Scene Heirarchy" } {}

	virtual void OnUpdate(f32 deltaTime) override;
	virtual void OnRender() override;

private:
	void Visit(Node* node);

private:
	u32 m_itemID{};
};

}

