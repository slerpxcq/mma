#include "CorePch.hpp"
#include "Node.hpp"

namespace mm
{

void Node::UpdateSubtreeWorldTransform() 
{
	for (auto& child : m_children) {
		// TODO: Check multiplication order
		child->m_worldTransform = m_worldTransform * child->m_localTransform;
		child->UpdateSubtreeWorldTransform();
	}
}

}
