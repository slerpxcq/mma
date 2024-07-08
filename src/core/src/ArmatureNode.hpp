#pragma once

#include "Node.hpp"
#include "BoneNode.hpp"

namespace mm
{

class ArmatureNode : public Node
{
	friend class ModelLoader;
public:
	ArmatureNode(const std::string& name = "", 
			   std::shared_ptr<Node> parent = nullptr) :
		Node(name, parent) {}

private:
	/* Linearized bones */
	std::vector<std::weak_ptr<BoneNode>> m_bones;
};

}

