#include "CorePch.hpp"
#include "SceneNode.hpp"

namespace mm
{

std::shared_ptr<SceneNode> SceneNode::CreateDefault()
{
	return std::shared_ptr<SceneNode>();
}

}
