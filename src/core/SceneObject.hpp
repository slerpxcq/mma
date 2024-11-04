#pragma once

namespace mm
{

class Node;

class SceneObject
{
public:
	virtual ~SceneObject() = default;
	SceneObject(StringView name) :
		m_name{ name } {}

	StringView GetName() const { return m_name; }
	Node* GetNode() const { return m_node; }
	void AttachTo(Node& node) { m_node = &node; }

protected:
	Node* m_node{};
	const String m_name{};
};

}

