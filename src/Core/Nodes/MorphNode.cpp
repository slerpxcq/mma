#include "mmpch.hpp"
#include "MorphNode.hpp"

namespace mm
{

std::unique_ptr<MorphNode> MorphNode::Make(const PMXFile& pmx)
{
	return std::make_unique<MorphNode>();
}

}
