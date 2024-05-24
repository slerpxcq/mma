#pragma once

#define REQUIRE_PARENT_TYPE(_parent_, _type_) MM_ASSERT(dynamic_cast<_type_*>(_parent_) && "Parent type must be " #_type_); 

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
	virtual void Attach(Node* parent) { m_parent = parent; }
	virtual void ShowProperties() {}
	virtual bool IsRemoveable() { return true; }

protected:
	std::string m_name;
	Node* m_parent = nullptr;
	std::vector<std::unique_ptr<Node>> m_children;
};

}

