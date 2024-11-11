#include "CorePch.hpp"
#include "Node.hpp"

namespace mm
{

// void Node::OnUpdate(f32 deltaTime)
// {
// 	for (auto& child : m_children) {
// 		child->OnUpdate(deltaTime);
// 	}
// 	for (auto& obj : m_objects) {
// 		obj->OnUpdate(deltaTime);
// 	}
// }

void Node::SetLocalTransform(const Transform& transform)
{
	m_localTransform = transform;
	InvalidateSubtreeWorldTransform();
}

void Node::SetWorldTransform(const Transform& transform)
{
	m_worldTransform = transform;
	CalculateLocalTransform();
	InvalidateSubtreeWorldTransform();
	m_isWorldTransformValid = true;
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

Transform Node::GetWorldTransform() 
{
	if (!m_isWorldTransformValid) {
		if (m_parent) {
			m_worldTransform = m_parent->GetWorldTransform() * m_localTransform;
		} else {
			m_worldTransform = m_localTransform;
		}
		m_isWorldTransformValid = true;
	}
	return m_worldTransform;
}

// void Node::UpdateSubtreeWorldTransform()
// {
// 	for (auto& child : m_children) {
// 		child->m_worldTransform = m_worldTransform * child->m_localTransform;
// 		child->UpdateSubtreeWorldTransform();
// 	}
// }

void Node::InvalidateSubtreeWorldTransform()
{
	m_isWorldTransformValid = false;
	for (auto& child : m_children) {
		child->InvalidateSubtreeWorldTransform();
	}
}

void Node::CalculateLocalTransform()
{
	if (m_parent) {
		m_localTransform = m_parent->GetWorldTransform().Inverse() * m_worldTransform;
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
