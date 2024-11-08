#include "CorePch.hpp"
#include "Node.hpp"

namespace mm
{

void Node::OnUpdate(f32 deltaTime)
{
	for (auto& child : m_children) {
		child->OnUpdate(deltaTime);
	}
	for (auto& obj : m_objects) {
		obj->OnUpdate(deltaTime);
	}
}

void Node::SetLocalTransform(const Transform& transform)
{
	m_localTransform = transform;
	// UpdateSubtreeWorldTransform();
}

void Node::SetWorldTransform(const Transform& transform)
{
	m_worldTransform = transform;
	CalculateLocalTransform();
	// UpdateSubtreeWorldTransform();
}

void Node::SetWorldTranslation(const Vec3& translation)
{
	SetWorldTransform({ translation, m_worldTransform.rotation });
}

void Node::SetWorldRotation(const Quat& rotation)
{
	SetWorldTransform({ m_worldTransform.translation, rotation });
}

void Node::SetLocalTranslation(const Vec3& translation)
{
	SetLocalTransform({ translation, m_localTransform.rotation });
}

void Node::SetLocalRotation(const Quat& rotation)
{
	SetLocalTransform({ m_localTransform.translation, rotation });
}

void Node::TransformWorld(const Transform& transform)
{
	SetWorldTransform(transform * m_worldTransform);
}

void Node::TransformLocal(const Transform& transform)
{
	SetLocalTransform(transform * m_localTransform);
}

void Node::TranslateLocal(const Vec3& translation)
{
	SetLocalTransform(m_localTransform + translation);
}

void Node::TranslateWorld(const Vec3& translation)
{
	SetWorldTransform(m_worldTransform + translation);
}

void Node::RotateLocal(const Quat& rotation)
{
	SetLocalTransform(rotation * m_localTransform);
}

void Node::RotateWorld(const Quat& rotation)
{
	SetWorldTransform(rotation * m_worldTransform);
}

void Node::UpdateSubtreeWorldTransform()
{
	for (auto& child : m_children) {
		/* TODO: Check multiplication order */
		child->m_worldTransform = m_worldTransform * child->m_localTransform;
		// child->m_worldTransform = child->m_localTransform * m_worldTransform ;
		child->UpdateSubtreeWorldTransform();
	}
}

void Node::CalculateLocalTransform()
{
	if (m_parent) {
		Transform localToWorld = m_parent->m_worldTransform;
		m_localTransform = localToWorld.Inverse() * m_worldTransform;
		// m_localTransform = localToWorld * m_worldTransform * localToWorld.Inverse();
	} else {
		m_localTransform = m_worldTransform;
	}
}

Node* Node::SearchChild(StringView name) 
{
	if (m_name == name) {
		return this;
	}

	for (auto& child : m_children) {
		if (Node* result{ child->SearchChild(name) }; result) {
			return result;
		}
	}

	return nullptr;
}

}
