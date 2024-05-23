#pragma once

#include "Node.hpp"

namespace mm
{

class PMXFile;

class MorphNode : public Node
{
	friend class ModelNode;
public:
	static std::unique_ptr<MorphNode> Make(const PMXFile& pmx);
};

}

