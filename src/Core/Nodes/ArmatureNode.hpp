#pragma once

#include "Node.hpp"

namespace mm
{

class PMXFile;

class ArmatureNode : public Node
{
	friend class ModelNode;
public:
	static std::unique_ptr<ArmatureNode> Make(const PMXFile& pmx);

private:
};

}

