#pragma once

namespace mm
{

class Node
{
	friend class SceneHierarchyPanel;
	friend class PropertiesPanel;
public:
	Node() {}
	Node(Node* parent) : m_parent(parent) {}
	virtual	~Node() {}
	virtual void ShowProperties() {}
	virtual bool IsRemoveable() { return true; }

protected:
	std::string m_name;
	const Node* m_parent;
	std::vector<std::unique_ptr<Node>> m_children;
};

}

