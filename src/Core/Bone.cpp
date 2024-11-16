#include "CorePch.hpp"
#include "Bone.hpp"

#include "Node.hpp"

namespace mm
{

void Bone::SetParent(Bone* parent)
{
	m_parent = parent;
	if (parent) {
		m_bindLocal = m_parent->GetBindWorldInverse() * m_bindWorld;
	}
	else {
		m_bindLocal = m_bindWorld;
	}
}

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

void Bone::SetLocalAxes(Vec3 x, Vec3 z)
{
	Vec3 y = glm::cross(z, x);
	m_localAxes = Mat3{ x, y, z };
}

}
