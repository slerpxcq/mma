#include "CorePch.hpp"
#include "Node.hpp"

namespace mm 
{

/* Recursively update world transform for the subtree */
void Node::UpdateWorldTransform()
{
	if (auto parent = m_parent.lock(); parent) {
		m_worldTransform = parent->m_worldTransform * m_localTransform;
	} else {
		m_worldTransform = m_localTransform;
	}

	for (auto child : m_children) {
		child->UpdateWorldTransform();
	}
}

}

