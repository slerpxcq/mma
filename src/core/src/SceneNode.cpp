#include "CorePch.hpp"
#include "SceneNode.hpp"

namespace mm
{

std::shared_ptr<SceneNode> SceneNode::CreateDefault(const std::string& name)
{
	return std::make_shared<SceneNode>(name);
}

}
