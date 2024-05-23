#pragma once

#include "Node.hpp"

namespace mm
{

class PMXFile;

class SkinNode : public Node
{
	friend class ModelNode;
public:
	static std::unique_ptr<SkinNode> Make(const PMXFile& pmx);

private:
};

}

