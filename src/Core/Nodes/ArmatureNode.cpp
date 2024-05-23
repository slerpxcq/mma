#include "mmpch.hpp"
#include "ArmatureNode.hpp"

namespace mm
{

std::unique_ptr<ArmatureNode> ArmatureNode::Make(const PMXFile& pmx)
{
	return std::make_unique<ArmatureNode>();
}

}

