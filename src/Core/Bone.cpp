#include "CorePch.hpp"
#include "Bone.hpp"

#include "Node.hpp"

namespace mm
{

void Bone::SetAnimLocal(const Transform& transform)
{
	m_animLocal = transform;
	m_node->SetLocalTransform(transform * m_bindLocal);
}

}
