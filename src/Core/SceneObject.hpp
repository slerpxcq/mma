#pragma once

#include "NamedObject.hpp"

namespace mm
{

class Node;

class SceneObject : public NamedObject
{
public:
	virtual ~SceneObject() = default;
	SceneObject(StringView name) :
		NamedObject{ name } {}

	virtual void OnUpdate(f32 deltaTime) {}

	Node* GetNode() const { return m_node; }
	void AttachTo(Node& node) { m_node = &node; }

protected:
	Node* m_node{};
};

}

