#pragma once

#include "NamedObject.hpp"

namespace mm
{

class Node;

class SceneObject : public NamedObject
{
public:
	MM_NONCOPYABLE(SceneObject)
	virtual ~SceneObject() = default;

	/* NOTE: Should not be called directly;
	*        Use SceneManager::CreateObject */
	SceneObject(StringView name) :
		NamedObject{ name } {}

	virtual void OnUpdate(f32 deltaTime) {}

	Node* GetNode() const { return m_node; }
	virtual void AttachTo(Node* node) { m_node = node; }

protected:
	Node* m_node{};
};

}

