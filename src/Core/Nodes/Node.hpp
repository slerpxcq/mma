#pragma once

namespace mm
{

class Node
{
	friend class SceneHierarchyPanel;
	friend class PropertiesPanel;
public:
	virtual	~Node() {}
	virtual void ShowProperties() {}

protected:
	std::string m_name;
	const Node* m_parent;
	std::vector<std::unique_ptr<Node>> m_children;
};

}

