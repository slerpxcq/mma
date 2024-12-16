#include "CorePch.hpp"
#include "Bone.hpp"

#include "Node.hpp"

#include "Physics/PhysicsManager.hpp"

#include "SceneManager.hpp"

namespace mm
{


Transform Bone::GetParentWorld()
{
	if (m_parent) {
		return m_parent->GetNode()->GetWorldTransform();
	} else {
		return Transform{};
	}
}

void Bone::SetAnimLocal(const Transform& transform)
{
	m_animLocal = transform;
	m_node->SetLocalTransform(m_bindLocal * transform);
}

Transform Bone::GetAnimLocal() 
{
	m_animLocal = m_bindLocal.Inverse() * m_node->GetLocalTransform();
	return m_animLocal;
}

Vec3 Bone::GetTipWorldPos() const
{
	if (m_flags & Bone::Flags::CONNECTED_BIT) {
		auto end = m_tipInfo.bone;
		if (end) {
			return end->GetNode()->GetWorldTranslation();
		} else {
			return m_node->GetWorldTranslation();
		}
	}
	else {
		auto transform = m_node->GetWorldTransform();
		return transform.translation + glm::rotate(transform.rotation, m_tipInfo.offset);
	}
}

void Bone::Builder::SetLocalAxes(Vec3 x, Vec3 z)
{
	Vec3 y = glm::cross(z, x);
	m_bone->m_localAxes = Mat3{ x, y, z };
}

void Bone::Builder::SetParent(Bone* parent)
{
	m_bone->m_parent = parent;
	if (parent) {
		m_bone->m_bindLocal = m_bone->m_parent->GetBindWorldInverse() * m_bone->m_bindWorld;
	}
	else {
		m_bone->m_bindLocal = m_bone->m_bindWorld;
	}
}

Bone::Builder::Builder(const Bone::ConstructInfo& info)
{
	m_bone = GetSceneManager()->CreateObject<Bone>(info);
}

}
