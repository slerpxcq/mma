#pragma once

namespace mm
{

class Node;

class SceneHierarchyPanel
{
public:
	void OnUIRender();

private:
	void Visit(Node* node);
};

}

