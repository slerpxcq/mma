#include "CorePch.hpp"
#include "Bone.hpp"

#include "Node.hpp"

namespace mm
{

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

}
