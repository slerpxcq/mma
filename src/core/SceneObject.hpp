#pragma once

namespace mm
{

class Node;

class SceneObject
{
public:
	SceneObject(StringView name) : m_name{ name } {}
	virtual ~SceneObject() = default;

	void AttachTo(Node& node) { m_node = &node; }
	StringView GetName() const { return m_name; }
private:
	Node* m_node{};
	const String m_name{};
};

}

