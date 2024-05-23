#include "mmpch.hpp"
#include "SkinNode.hpp"

namespace mm
{

std::unique_ptr<SkinNode> SkinNode::Make(const PMXFile& pmx)
{
	return std::make_unique<SkinNode>();
}

}

