#pragma once

#include "Node.hpp"

namespace mm
{

class PMXFile;
class SkinNode;
class ArmatureNode;
class MorphNode;

class ModelNode : public Node
{
public:
	static std::unique_ptr<ModelNode> Make(const PMXFile& pmx);

private:
	SkinNode* m_skinNode = nullptr;
	ArmatureNode* m_armatureNode = nullptr;
	MorphNode* m_morphNode = nullptr;
};

}

