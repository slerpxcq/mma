#include "mmpch.hpp"
#include "RootNode.hpp"

#include "SceneNode.hpp"

namespace mm
{
void RootNode::Startup()
{
	m_name = "Root";
	auto scene = std::make_unique<SceneNode>(this);
	scene->m_name = "DefaultScene";
	m_children.push_back(std::move(scene));
}

void RootNode::Shutdown()
{
}

}

